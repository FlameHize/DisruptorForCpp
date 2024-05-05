// Copyright (c) 2024, zgx
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the disruptor-- nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCOIS SAINT-JACQUES BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DISRUPTOR_CLAIM_STRATEGY_H_
#define DISRUPTOR_CLAIM_STRATEGY_H_

#include <thread>
#include "sequence.h"
#include "ring_buffer.h"

namespace disruptor {
// Claim Startegy Option
enum ClaimStrategyOption
{
    kSingleThreadClaimStrategy,
    kMultiThreadClaimStrategy
};

// Interface of ClaimStrategy
class ClaimStrategy
{
public:
    /**
     * @brief Wait for the given sequence to be available for consumption
     * @param dependents  dependents sequences to wait on (mostly consumers).
     * @param delta       sequences to claim [default: 1].
     * @return last claimed sequence
    */
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta = 1) = 0;

    /**
     * @brief Determine if there is enough space in the circular buffer
     * @param dependents Set of queues waiting for consumption of circular buffer data
    */
    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) = 0;

    /**
     * @brief Published event
     * @param sequence Used for MultiThreadStrategy to update available buffer
     * @param delta Used for SingleThreadStrategy to update cursor
    */
    virtual void Publish(const int64_t& sequence) = 0;

    /**
     * @brief Judge whether the sequence is available
    */
    virtual bool IsAvailable(const int64_t& sequence) = 0;

    /**
     * Get the highest sequence number that can be safely read from the ring buffer.  
     * The scan will range from nextSequence to availableSequence.  
     * If there are no available values > nextSequence the return value will be nextSequence - 1.
     * To work correctly a consumer should pass a value that it 1 higher than the last sequence that was successfully processed.
     *
     * @param lowerBound The sequence to start scanning from.
     * @param availableSequence The sequence to scan to.
     * @returns The highest value that can be safely read, will be at least nextSequence - 1
    */
    virtual int64_t GetHighesetPublishedSequence(int64_t low_bound,
                            int64_t available_sequence) = 0;
};

// used internally
// inline function allow multi define in file
static inline ClaimStrategy* CreateClaimStrategy(ClaimStrategyOption option,
                                                 int64_t buffer_size,
                                                 Sequence& cursor);

// Apply to a single publisher thread
// Optimised strategy can be used when there is a single publisher thread.
class SingleThreadStrategy : public ClaimStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(SingleThreadStrategy);
public:
    SingleThreadStrategy(int64_t buffer_size,Sequence& cursor) :
        _cursor(cursor),
        _buffer_size(buffer_size),
        _cursor_sequence_cache(kInitialCursorValue),
        _gating_sequence_cache(kInitialCursorValue) {}
    
    // producer batch processing
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta) override {
        // Get producer cursor and calcualte next available sequence
        // _cursor_sequence_cache is used to cached replace for cursor's sequence
        _cursor_sequence_cache += delta;
        
        // Calculate overlap point to prevent ring buffer wrapping
        const int64_t wrap_point = _cursor_sequence_cache - _buffer_size;

        // If the wrap_point is greater than the cached _gating_sequence_cache, 
        // it indicates that some consumers have not completed the processing and need to wait
        if(wrap_point > _gating_sequence_cache) {
            int64_t min_sequence;
            // Waiting for non overlapping
            while(wrap_point > (min_sequence = GetMinimumSequence(dependents))) {
                std::this_thread::yield();
            }
            // Cache the minimum serial number of consumers
            _gating_sequence_cache = min_sequence;
        }
        return _cursor_sequence_cache;
    }

    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) override {
        const int64_t wrap_point = _cursor_sequence_cache - _buffer_size + 1L;
        if(_gating_sequence_cache < wrap_point) {
            // Update once comsumer's sequence if the consumer's 
            // sequence is already lower than wrap_point,it means
            // there is no avail space currently
            _gating_sequence_cache = GetMinimumSequence(dependents);
            if(_gating_sequence_cache < wrap_point) {
                return false;
            }
        }
        return true;
    }

    virtual void Publish(const int64_t& sequence) override {
        _cursor.SetSequence(sequence);
    }

    virtual bool IsAvailable(const int64_t& sequence) override {
        return sequence <= _cursor.GetSequence();
    }

    virtual int64_t GetHighesetPublishedSequence(int64_t low_bound,
                                                 int64_t available_sequence) override {
        return available_sequence;
    }

private:
    Sequence& _cursor;
    int64_t _buffer_size;
    int64_t _cursor_sequence_cache;
    int64_t _gating_sequence_cache;
};

// Apply to multi publisher thread
// Optimised strategy can be used when there is a single publisher thread.
class MultiThreadStrategy : public ClaimStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(MultiThreadStrategy);
public:
    MultiThreadStrategy(int64_t buffer_size,Sequence& cursor) :
        _cursor(cursor), 
        _buffer_size(buffer_size),
        _available_buffer(new int64_t[buffer_size]) {
        _index_mask = buffer_size - 1;
        _index_shift = util::Log2(buffer_size);
        InitialAvailableBuffer();
    }

    // May be used for mulit producers at the same time 
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta) override {
        // Try get next sequence
        int64_t current_sequence;
        int64_t next_sequence;
        while(true) {
            // Get cursor and expect value
            current_sequence = _cursor.GetSequence();
            next_sequence = current_sequence + delta;
            
            // Calculate overlap point to prevent ring buffer wrapping
            int64_t wrap_point = next_sequence - _buffer_size;

            // Get cached minimum gating sequence
            int64_t cached_gating_sequence = _gating_sequence_cache.GetSequence();

            // If the wrap_point is greater than the cached _gating_sequence_cache, 
            // it indicates that some consumers have not completed the processing and need to wait
            if(wrap_point > cached_gating_sequence) {
                // Get the last consumers sequence
                int64_t min_sequence = GetMinimumSequence(dependents);

                // Still overlap
                if(wrap_point > min_sequence) {
                    std::this_thread::yield();
                    continue;
                }
                // If is not overlap,update cached_gating_sequence(last_consumer_sequence)
                _gating_sequence_cache.SetSequence(min_sequence);
            }
            // No overlap,directly set the _cursor to next_sequence
            else if(_cursor.CompareAndSet(current_sequence,next_sequence)) {
                break;
            }
        }
        return next_sequence;
    }

    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) override {
        const int64_t wrap_point = _cursor.GetSequence() - _buffer_size + 1L;
        if(_gating_sequence_cache.GetSequence() < wrap_point) {
            const int64_t min_sequence = GetMinimumSequence(dependents);
            _gating_sequence_cache.SetSequence(min_sequence);
            if(min_sequence < wrap_point) {
                return false;
            }
        }
        return true;
    }

    virtual void Publish(const int64_t& sequence) override {
        SetAvailable(sequence);
    }

    virtual int64_t GetHighesetPublishedSequence(int64_t low_bound,
                                                 int64_t available_sequence) override {
        for(int64_t sequence = low_bound; sequence <= available_sequence; ++sequence) {
            if(!IsAvailable(sequence)) {
                return sequence - 1;
            }
        }
        return available_sequence;
    }

    virtual bool IsAvailable(const int64_t& sequence) override {
        int64_t index = CalculateIndex(sequence);
        int64_t flag = CalculateAvailableFlag(sequence);
        return _available_buffer[index] == flag;
    }

private:
    void InitialAvailableBuffer() {
        for(int64_t sequence = 0; sequence < _buffer_size; ++sequence) {
            SetAvailableBufferFlag(sequence,-1);
        }
    }

    void SetAvailableBufferFlag(int64_t index,int64_t flag) {
        _available_buffer[index] = flag;
    }

    void SetAvailable(int64_t sequence) {
        SetAvailableBufferFlag(CalculateIndex(sequence),CalculateAvailableFlag(sequence));
    }

    int64_t CalculateIndex(int64_t sequence) {
        return sequence & _index_mask;
    }

    int64_t CalculateAvailableFlag(int64_t sequence) {
        return static_cast<int64_t>(static_cast<uint64_t>(sequence) >> _index_shift);
    }

private:
    Sequence& _cursor;
    int64_t _buffer_size;
    Sequence _gating_sequence_cache;

    int64_t* _available_buffer;
    int64_t _index_mask;
    int64_t _index_shift;
};

static inline ClaimStrategy* CreateClaimStrategy(ClaimStrategyOption option,int64_t buffer_size,Sequence& cursor) {
    ClaimStrategy* strategy = nullptr;
    switch (option) {
    case kSingleThreadClaimStrategy:
        strategy = new SingleThreadStrategy(buffer_size,cursor);
        break;
    case kMultiThreadClaimStrategy:
        strategy = new MultiThreadStrategy(buffer_size,cursor);
        break;
    default:
        break;
    }
    return strategy;
}

} // end namespace disruptor

#endif