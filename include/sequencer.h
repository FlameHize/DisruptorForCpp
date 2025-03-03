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

#ifndef DISRUPTOR_SEQUENCER_H_
#define DISRUPTOR_SEQUENCER_H_

#include "ring_buffer.h"
#include "sequence.h"
#include "claim_strategy.h"
#include "wait_strategy.h"
#include "sequence_barrier.h"

namespace disruptor {
/**
 * @brief Two stage submission claim and publish
 *      int64_t sequence = Next();
 *      Sequencer[sequence].value = user_setting_value;
 *      Publish();
*/
template<typename T>
class Sequencer
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(Sequencer);
public:
    // Construct a Sequencer with the selected strategies
    explicit Sequencer(int64_t buffer_size = kDefaultRingBufferSize,
                       ClaimStrategyOption claim_option = kSingleThreadClaimStrategy,
                       WaitStrategyOption wait_option = kBusySpinStrategy) 
        : _ring_buffer(buffer_size),
          _claim_strategy(CreateClaimStrategy(claim_option,buffer_size,_cursor)),
          _wait_strategy(CreateWaitStrategy(wait_option)) {}

    // Set the sequences(consumers) that will gate producers to prevent
    // the ring buffer wrapping
    // sequences are the last level consumers in the processing
    void SetGatingSequences(const std::vector<Sequence*>& sequences) {
        _gating_sequences = sequences;
    }

    // Get the value of the cursor indicating the published sequence
    int64_t GetCursor() {
        return _cursor.GetSequence();
    }

    // Create a barrier that gates on the cursor and a list of Sequences
    SequenceBarrier* NewBarrier(const std::vector<Sequence*>& dependents) {
        return new SequenceBarrier(_cursor,dependents,_wait_strategy,_claim_strategy);
    }

    bool HasAvailableCapacity() {
        return _claim_strategy->HasAvailableCapacity(_gating_sequences);
    }

    // Claim the next batch of sequence number for publishing
    // return the next available sequence
    int64_t Next(size_t delta = 1) {
        return _claim_strategy->IncrementAndGet(_gating_sequences,delta);
    }

    /// @brief Used for producer to publish events
    /// @param sequence maximum sequence of events to be published
    void Publish(const int64_t& sequence) {
        _claim_strategy->Publish(sequence);
        
        // notify the consumers to obtain new event
        _wait_strategy->SignalAllWhenBlocking();
    }

    void Publish(int64_t low_bound, int64_t high_bound) {
        _claim_strategy->Publish(low_bound, high_bound);
        _wait_strategy->SignalAllWhenBlocking();
    }

    // Get value use operator[]
    T* operator[](const int64_t& sequence) {
        return _ring_buffer[sequence];
    }

private:
    RingBuffer<T> _ring_buffer;
    Sequence _cursor;
    ClaimStrategy* _claim_strategy;
    WaitStrategy* _wait_strategy;

    /**
     * Each consumer will maintain their own Sequence object to 
     * record the sequence positions they have already consumed.
     * Every time a consumer is added, their Sequence reference 
     * is added to _gating_sequences.
     * By accessing _gating_sequences, the Sequencer can determine 
     * where the slowest consumer has spent.
    */
    // Records the sequence of consumers
    std::vector<Sequence*> _gating_sequences;
};
} // end namespace disruptor

#endif