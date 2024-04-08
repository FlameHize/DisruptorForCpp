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

#ifndef DISRUPTOR_CONSUMER_BARRIER_H_
#define DISRUPTOR_CONSUMER_BARRIER_H_

#include <memory>
#include <vector>

#include "sequence.h"
#include "wait_strategy.h"

namespace disruptor {

/**
 * @brief Used for consumer to wait for the target sequence
 * @example int64_t available_sequence = ConsumerBarrier.WaitFor(next_sequence);
*/
template<typename W = kDefaultWaitStrategy>
class ConsumerBarrier
{
public:
    explicit ConsumerBarrier(const Sequence& cursor,
                             const std::vector<Sequence*>& dependents)
    : _cursor(cursor),_dependents(dependents),_alerted(false) {}

    /**
     * @brief Return the maximum accessible serial number of RingBuffer
    */
    inline int64_t WaitFor(const int64_t& sequence) {
        return _wait_strategy.WaitFor(sequence,_cursor,_dependents,_alerted);
    }

    template<typename Rep,typename Period>
    inline int64_t WaitFor(const int64_t& sequence,
                           const std::chrono::duration<Rep,Period>& timeout) {
        return _wait_strategy.WaitFor(sequence,_cursor,_dependents,_alerted,timeout);
    }

    inline int64_t GetSequence() {
        return _cursor.GetSequence();
    }

    inline bool Alerted() const {
        return _alerted.load(std::memory_order::memory_order_acquire);
    }

    inline void SetAlerted(bool alert) {
        _alerted.store(alert,std::memory_order::memory_order_release);
    }
private:
    // producer
    const Sequence& _cursor;
    // current consumer(which use this barrier) dependents's condition
    std::vector<Sequence*> _dependents;
    // alerted
    std::atomic<bool> _alerted;
    // strategy decide how it will wait for this available sequence
    W _wait_strategy;
};

} // end namespace disruptor

#endif