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

#ifndef DISRUPTOR_SEQUENCE_TEST_H_
#define DISRUPTOR_SEQUENCE_TEST_H_

#include <gtest/gtest.h>
#include "sequence.h"

namespace disruptor {
namespace test {

    TEST(SequenceTest,SequenceStartWithInitialed)
    {
        Sequence seq;
        EXPECT_EQ(seq.GetSequence(),kInitialCursorValue);

        seq.SetSequence(2L);
        EXPECT_EQ(seq.GetSequence(),2L);

        EXPECT_EQ(seq.IncrementAndGet(1L),3L);
        EXPECT_EQ(seq.GetSequence(),3L);

        EXPECT_EQ(seq.IncrementAndGet(3L),6L);
        EXPECT_EQ(seq.GetSequence(),6L);
    }

    TEST(SequenceTest,SequenceAtLeastOneCacheLine)
    {
        EXPECT_GE(sizeof(Sequence),CACHE_LINE_SIZE_IN_BYTES);
    }

    TEST(SequenceTest,SequenceIsCacheLineAligned)
    {
        // Alignof is used to calculate the alignment requirements 
        // for the maximum element in the memory alignment of 
        // base variables, structures, and classes
        // Make sure the Sequnece is aligned base of 8 byte
        EXPECT_EQ(alignof(Sequence),CACHE_LINE_SIZE_IN_BYTES / 8);
    }

} // end namespace test
} // end namespace disruptor

#endif