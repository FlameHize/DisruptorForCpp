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

#ifndef DISRUPTOR_SEQUENCE_H_
#define DISRUPTOR_SEQUENCE_H_

#ifndef CACHE_LINE_SIZE_IN_BYTES
#define CACHE_LINE_SIZE_IN_BYTES 64
#endif

// for atomic param cache line padding 7
#define ATOMIC_SEQUENCE_PADDING_LENGTH \
    (CACHE_LINE_SIZE_IN_BYTES - sizeof(std::atomic<int64_t>)) / 8

#include <atomic>
#include <vector>
#include <limits.h>
#include "utils.h"

namespace disruptor {

// special cursor values
constexpr int64_t kInitialCursorValue = -1L;
constexpr int64_t kAlertedSignal = -2L;
constexpr int64_t kTimeoutSignal = -3L;
constexpr int64_t kFirstSequenceValue = kInitialCursorValue + 1L;

class Sequence
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(Sequence);
public:
    // Construct a sequence counter that can be tracked accross threads
    explicit Sequence(int64_t initial_value = kInitialCursorValue)
        : _sequence(initial_value) {}

    // Get the current value of the Sequence
    int64_t GetSequence() const {
        return _sequence.load(std::memory_order::memory_order_acquire);
    }

    // Set the current value of the Sequence
    void SetSequence(int64_t value) {
        _sequence.store(value,std::memory_order::memory_order_release);
    }

    // Increment and return the increased value of the sequence
    int64_t IncrementAndGet(const int64_t& increment) {
        return _sequence.fetch_add(increment,std::memory_order::memory_order_release) + increment;
    }
private:
    // padding make sure the _sequece won't appear with other param
    int64_t _padding0[ATOMIC_SEQUENCE_PADDING_LENGTH];
    // member
    std::atomic<int64_t> _sequence;
    // padding
    int64_t _padding1[ATOMIC_SEQUENCE_PADDING_LENGTH];
};

inline int64_t GetMinimumSequence(const std::vector<Sequence*>& sequences) 
{
    int64_t minimum = LONG_MAX;
    for(Sequence* sequence : sequences) {
        const int64_t sequence_value = sequence->GetSequence();
        minimum = std::min(minimum,sequence_value);
    }
    return minimum;
}

} // end namespace disruptor
#endif

/**
 * Memory fence: To solve the compiler's instruction rearrangement
 * (1) memory_order::memory_order_release: 
 *      Do not rearrange the previous memory access instruction after this 
 *      instruction. When the result of this instruction is visible to other 
 *      threads, all previous instructions are visible
 * (2) memory_order::memory_order_acquire:
 *      Do not rearrange subsequent memory access instructions before this instruction
 * example:
 *      Thread1
 *      p.init();
 *      ready.store(true,std::memory_order_release);
 * 
 *      Thread2
 *      if(ready.load(std::memory_order_acquire))
 *          p.bar();
 * 
 *      when ready == true can be visiable to thread2,it means thread2 also can
 *      see the p.init() instruction(guarantee by memory_order_release instruction)
 *      The memory_order_acquire used to make sure the p.bar() instruction
 *      after ready.load() 
 * 
 *      when set the atomic param, use store and memory_order_release 
 *      to make sure the pre instruction is completed
 *      when get the atomic param, use load and memory_order_acquired
 *      to make sure the value get is first finished, then allow complier
 *      use this value to complete follow-up operations
*/
