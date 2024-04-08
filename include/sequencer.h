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
#include "consumer_barrier.h"

namespace disruptor {
/**
 * @brief Two stage submission claim and publish
 *      int64_t sequence = Next();
 *      Sequencer[sequence].value = user_setting_value;
 *      Publish();
 * @todo 不从模板暴露过多参数 仅暴露T Sequencer参数 N C W
*/
template<typename T,size_t N = kDefaultRingBufferSize,
         typename C = kDefaultClaimStrategy,typename W = kDefaultWaitStrategy>
class Sequencer
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(Sequencer);
public:
    // Construct a Sequencer with the selected strategies
    explicit Sequencer() : _ring_buffer(N) {}

    // Set the sequences(consumers) that will gate publishers to prevent
    // the ring buffer wrapping
    void SetGatingSequences(const std::vector<Sequence*>& sequences) {
        _gating_sequences = sequences;
    }

    // Get the value of the cursor indicating the published sequence
    int64_t GetCursor() {
        return _cursor.GetSequence();
    }

    // Create a barrier that gates on the cursor and a list of Sequences
    ConsumerBarrier<W>* NewBarrier(const std::vector<Sequence*>& dependents) {
        return new ConsumerBarrier<W>(_cursor,dependents);
    }

    bool HasAvailableCapacity() {
        return _claim_strategy.HasAvailableCapacity(_gating_sequences);
    }

    // Claim the next batch of sequence number for publishing
    // return the next available sequence
    int64_t Next(size_t delta = 1) {
        return _claim_strategy.IncrementAndGet(_gating_sequences,delta);
    }

    /// @brief Used for producer to publish events
    /// @param sequence maximum sequence of events to be published
    /// @param delta num of events to be published
    void Publish(const int64_t& sequence,size_t delta = 1) {
        // guard for mutli producer publish at the same time
        _claim_strategy.SynchronizePublishing(sequence,_cursor,delta);
        // update cursor and signal comsumers
        _cursor.IncrementAndGet(delta);
        // notify the consumers to obtain new event
        _wait_strategy.SignalAllWhenBlocking();
    }

    // Get value
    T& operator[](const int64_t& sequence) {
        return _ring_buffer[sequence];
    }

private:
    RingBuffer<T> _ring_buffer;
    Sequence _cursor;
    C _claim_strategy;
    W _wait_strategy;

    /**
     * @note
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