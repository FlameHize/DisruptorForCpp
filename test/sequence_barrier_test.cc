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

#ifndef DISRUPTOR_CONSUMER_BARRIER_TEST_H_
#define DISRUPTOR_CONSUMER_BARRIER_TEST_H_

#include <gtest/gtest.h>
#include "consumer_barrier.h"

namespace disruptor {
namespace test {

class ConsumerBarrierTest : public testing::Test
{
public:
    ConsumerBarrierTest() : barrier(cursor,dependents) {}

    Sequence cursor;
    Sequence sequence_1;
    Sequence sequence_2;
    Sequence sequence_3;
    std::vector<Sequence*> dependents;
    ConsumerBarrier<> barrier;

    std::vector<Sequence*> AllDependents() {
        std::vector<Sequence*> result;
        result.push_back(&sequence_1);
        result.push_back(&sequence_2);
        result.push_back(&sequence_3);
        return result;
    }
};

TEST_F(ConsumerBarrierTest,BasicSetAndGet) 
{
    EXPECT_EQ(barrier.Alerted(),false);
    barrier.SetAlerted(true);
    EXPECT_EQ(barrier.Alerted(),true);
}

TEST_F(ConsumerBarrierTest,WaitForCursor)
{
    // set a value to save result
    std::atomic<int64_t> return_value(kInitialCursorValue);

    // mock for consumer to wait the sequence on another thread
    std::thread waiter([this,&return_value](){
        return_value.store(barrier.WaitFor(kFirstSequenceValue));
    });
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    // mock for producer to publish sequnece on another thread
    ///@bug if the strategy is BlockingStrategy,no signal notify
    std::thread ([this](){
       cursor.IncrementAndGet(1L); 
    }).join();
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);

    std::thread waiter2([this,&return_value](){
        return_value.store(barrier.WaitFor(kFirstSequenceValue + 1L,
                                           std::chrono::seconds(5)));
    });
    std::thread ([this](){
        cursor.IncrementAndGet(1L);
    }).join();
    waiter2.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue + 1L);
}

} // end namespace test
} // end namespace disruptor

#endif