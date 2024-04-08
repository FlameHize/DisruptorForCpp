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

// Interface of ClaimStrategy
class ClaimStrategy
{
    /**
     * @brief Wait for the given sequence to be available for consumption
     * @param dependents  dependents sequences to wait on (mostly consumers).
     * @param delta       sequences to claim [default: 1].
     * @return last claimed sequence
    */
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta) = 0;

    /**
     * @brief Determine if there is enough space in the circular buffer
     * @param dependents Set of queues waiting for consumption of circular buffer data
    */
    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) = 0;

    /**
     * @brief Guard for mutli producer race condition
     * @param sequence Producer's get maximum available number
     * @param cursor Point to the next writable position in the circular buffer
     * @param delta Num of events to be published
     * @cite sequence-delta: sequence of the first event to be published 
    */
    virtual void SynchronizePublishing(const int64_t& sequence,const Sequence& cursor,
                               const size_t& delta) = 0;
};

template<size_t N>
class SingleThreadStrategy;
using kDefaultClaimStrategy = SingleThreadStrategy<kDefaultRingBufferSize>;

// Apply to a single publisher thread
// Optimised strategy can be used when there is a single publisher thread.
template<size_t N = kDefaultRingBufferSize>
class SingleThreadStrategy : public ClaimStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(SingleThreadStrategy);
public:
    SingleThreadStrategy() :
        _last_claimed_sequence(kInitialCursorValue),
        _last_consumer_sequence(kInitialCursorValue) {}
    
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta = 1) override {
        _last_claimed_sequence += delta;
        // Prevent ring buffer wrap
        // Waiting for consumers to complete their consumption until circle diff
        const int64_t wrap_point = _last_claimed_sequence - N;
        if(_last_consumer_sequence < wrap_point) {
            // The dependents in this place means consumers? 
            while(GetMinimumSequence(dependents) < wrap_point) {
                std::this_thread::yield();
            }
        }
        return _last_claimed_sequence;
    }

    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) override {
        const int64_t wrap_point = _last_claimed_sequence - N + 1L;
        if(_last_consumer_sequence < wrap_point) {
            // Update once comsumer's sequence if the consumer's 
            // sequence is already lower than wrap_point,it means
            // there is no avail space currently
            _last_consumer_sequence = GetMinimumSequence(dependents);
            if(_last_consumer_sequence < wrap_point) {
                return false;
            }
        }
        return true;
    }

    virtual void SynchronizePublishing(const int64_t& sequence,const Sequence& cursor,
                                       const size_t& delta) override {
        // empty temp
    }
private:
    // we do not need to use atomic values since this function is called
    // by a single publisher thread
    int64_t _last_claimed_sequence;
    int64_t _last_consumer_sequence;
};

// Apply to multi publisher thread
// Optimised strategy can be used when there is a single publisher thread.
template<size_t N = kDefaultRingBufferSize>
class MultiThreadStrategy : public ClaimStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(MultiThreadStrategy);
public:
    MultiThreadStrategy() {}

    // May be used for mulit producers at the same time 
    virtual int64_t IncrementAndGet(const std::vector<Sequence*>& dependents,
                                    size_t delta = 1) override {
        // CAS operation race condition
        const int64_t next_sequence = _last_claimed_sequence.IncrementAndGet(delta);
        const int64_t wrap_point = next_sequence - N;
        if(_last_consumer_sequence.GetSequence() < wrap_point) {
            while(GetMinimumSequence(dependents) < wrap_point) {
                std::this_thread::yield();
            }
        }
        return next_sequence;
    }

    virtual bool HasAvailableCapacity(const std::vector<Sequence*>& dependents) override {
        const int64_t wrap_point = _last_claimed_sequence.GetSequence() -N + 1L;
        if(_last_consumer_sequence.GetSequence() < wrap_point) {
            const int64_t min_sequence = GetMinimumSequence(dependents);
            _last_consumer_sequence.SetSequence(min_sequence);
            if(min_sequence < wrap_point) {
                return false;
            }
        }
        return true;
    }

    // Guard for mutli producer
    virtual void SynchronizePublishing(const int64_t& sequence,const Sequence& cursor,
                                       const size_t& delta) override {
        // sequence: producer's get maximum available number
        // delta: num of events to be published
        // sequence - delta : sequence of the first event to be published 
        int64_t first_sequence = sequence - delta;
        while(cursor.GetSequence() < first_sequence) {
            std::this_thread::yield();
        }
    }
private:
    Sequence _last_claimed_sequence;
    Sequence _last_consumer_sequence;
};

} // end namespace disruptor

#endif