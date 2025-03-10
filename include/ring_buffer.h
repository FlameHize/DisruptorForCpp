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

#ifndef DISRUPTOR_RING_BUFFER_H_
#define DISRUPTOR_RING_BUFFER_H_

#include <array>
#include <utils.h>

namespace disruptor {
constexpr size_t kDefaultRingBufferSize = 1024;

/**
 * @brief RingBuffer implemented with a fixed array
 * @param T EventType
 */
template <typename T>
class RingBuffer
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(RingBuffer);
public:
    explicit RingBuffer(int64_t size) : _size(size), _events(new T[size]) {
        // assert(((size > 0) && ((size & (~size + 1)) == size)),
        //             "RingBuffer's size must be a positive power of 2");
    }

    ~RingBuffer() {
        delete []_events;
        _events = nullptr;
    }

    /**
     * @brief Get the event for a given sequence in the RingBuffer
     * @param sequence sequence for the event(increase from zero)
     * @return event referenced at the specified sequence position
     */
    T* operator[](const int64_t& sequence) { 
        return &_events[sequence & (_size - 1)]; 
    }

private:
    int64_t _size;
    T* _events;
};

}
// end namepsace disruptor

#endif