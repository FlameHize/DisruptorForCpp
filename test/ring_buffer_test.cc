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

#ifndef DISRUPTOR_RING_BUFFER_TEST_H_
#define DISRUPTOR_RING_BUFFER_TEST_H_

#include <gtest/gtest.h>
#include "ring_buffer.h"

namespace disruptor {
namespace test {

class RingBufferTest : public testing::Test
{
public:
    static constexpr int kTestRingBufferSize = 8;
    
    RingBufferTest() : ring_buffer(initArray()) {}

    std::array<int,kTestRingBufferSize> initArray() {
        std::array<int,kTestRingBufferSize> temp;
        for(size_t i = 0; i < kTestRingBufferSize; ++i) {
            temp[i] = index(i);
        }
        return temp;
    }

    size_t index(const size_t index) { 
        return index + 1;
    }

    RingBuffer<int,kTestRingBufferSize> ring_buffer;
};

TEST_F(RingBufferTest,RingBufferIndexCalculate)
{
    // we can use the RingBufferTest's data and func
    for(size_t i = 0; i < kTestRingBufferSize * 2; ++i) {
        EXPECT_EQ(ring_buffer[i], index(i % kTestRingBufferSize));
    }
}

TEST_F(RingBufferTest,RingBufferOperator)
{
    for(size_t i = 0; i < kTestRingBufferSize * 2; ++i) {
        int index = i & (kTestRingBufferSize - 1);
        int num = index + 1;
        EXPECT_EQ(num,ring_buffer[i]);
    }
}

} // end namespace test

} // end namespace disruptor
#endif