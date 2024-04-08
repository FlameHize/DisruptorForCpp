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

#ifndef DISRUPTOR_WAIT_STRATEGY_TEST_H_
#define DISRUPTOR_WAIT_STRATEGY_TEST_H_

#include <thread>
#include <gtest/gtest.h>
#include "wait_strategy.h"

namespace disruptor {
namespace test {

template<typename Strategy>
class WaitStrategyTest : public testing::Test
{
public:
    WaitStrategyTest() : alerted(false) {}

    // producer
    Sequence cursor;

    // consumer
    Sequence sequence_1;
    Sequence sequence_2;
    Sequence sequence_3;
    std::vector<Sequence*> dependents;
    std::atomic<bool> alerted;
    Strategy strategy;

    std::vector<Sequence*> AllDependents() {
        std::vector<Sequence*> result;
        result.push_back(&sequence_1);
        result.push_back(&sequence_2);
        result.push_back(&sequence_3);
        return result;
    }
};

using BusySpinStrategyTest = WaitStrategyTest<BusySpinStrategy>;

TEST_F(BusySpinStrategyTest,WaitForCursor)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    
    std::thread waiter([this,&return_value](){
        // init first cursor sequence is -1
        // run thread,expect return_value is kInitialCursorValue
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted));
    });

    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    std::thread([this](){
        // set the cursor increased to kFirstSequenceValue
        cursor.IncrementAndGet(1L);
        // strategy.SignalAllWhenBlocking();
    }).join();
    waiter.join();

    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BusySpinStrategyTest,WaitForTimeout)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                            std::chrono::microseconds(1L)));
    });
    waiter.join();
    EXPECT_EQ(return_value.load(),kTimeoutSignal);

    std::thread waiter2([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                            std::chrono::seconds(1L)));
    });

    cursor.IncrementAndGet(1L);
    // strategy.SignalAllWhenBlocking();
    waiter2.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BusySpinStrategyTest,WaitForDependents)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
                                            AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    // strategy.SignalAllWhenBlocking();
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_3.IncrementAndGet(1L);
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BusySpinStrategyTest,WaitForDependentsWithAlert)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
                                            AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    // strategy.SignalAllWhenBlocking();

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    alerted.store(true);
    waiter.join();
    EXPECT_EQ(return_value.load(),kAlertedSignal);
}

using YieldingStrategyTest = WaitStrategyTest<YieldingStrategy<>>;

TEST_F(YieldingStrategyTest,WaitForCursor)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted));
    });
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    std::thread([this](){
        cursor.IncrementAndGet(1L);
        strategy.SignalAllWhenBlocking();
    }).join();
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(YieldingStrategyTest,WaitForTimeout)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                           std::chrono::microseconds(1L)));
    });
    waiter.join();
    EXPECT_EQ(return_value.load(),kTimeoutSignal);

    std::thread waiter2([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                           std::chrono::seconds(1L)));
    });
    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();
    waiter2.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}   

TEST_F(YieldingStrategyTest,WaitForDependents)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();

    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_3.IncrementAndGet(1L);
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(YieldingStrategyTest,WaitForDependentsWithAlert)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();

    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    alerted.store(true);
    waiter.join();
    EXPECT_EQ(return_value.load(),kAlertedSignal);
}

using SleepingStrategyTest = WaitStrategyTest<SleepingStrategy<>>;

TEST_F(SleepingStrategyTest,WaitForCursor)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted));
    });
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    std::thread([this](){
        cursor.IncrementAndGet(1L);
        strategy.SignalAllWhenBlocking();
    }).join();
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(SleepingStrategyTest,WaitForTimeout)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                           std::chrono::microseconds(1L)));
    });
    waiter.join();
    EXPECT_EQ(return_value.load(),kTimeoutSignal);

    std::thread waiter2([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted,
                                           std::chrono::seconds(1L)));
    });
    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();
    waiter2.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}   

TEST_F(SleepingStrategyTest,WaitForDependents)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();

    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_3.IncrementAndGet(1L);
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(SleepingStrategyTest,WaitForDependentsWithAlert)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,AllDependents(),alerted));
    });

    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();

    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    alerted.store(true);
    waiter.join();
    EXPECT_EQ(return_value.load(),kAlertedSignal);
}

using BlockingStrategyTest = WaitStrategyTest<BlockingStrategy>;

TEST_F(BlockingStrategyTest,WaitForCursor)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,dependents,alerted));
    });
    EXPECT_EQ(return_value.load(),kInitialCursorValue);
    std::thread([this](){
        cursor.IncrementAndGet(1L);
        strategy.SignalAllWhenBlocking();
    }).join();
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BlockingStrategyTest,WaitForTimeout)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
        dependents,alerted,std::chrono::microseconds(1L)));
    });
    waiter.join();
    EXPECT_EQ(return_value.load(),kTimeoutSignal);
    std::thread waiter2([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
        dependents,alerted,std::chrono::seconds(1L)));
    });
    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();
    waiter2.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BlockingStrategyTest,WaitForDependents)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
        AllDependents(),alerted));
    });
    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_3.IncrementAndGet(1L);
    waiter.join();
    EXPECT_EQ(return_value.load(),kFirstSequenceValue);
}

TEST_F(BlockingStrategyTest,WaitForDependentsWithAlert)
{
    std::atomic<int64_t> return_value(kInitialCursorValue);
    std::thread waiter([this,&return_value](){
        return_value.store(strategy.WaitFor(kFirstSequenceValue,cursor,
        AllDependents(),alerted));
    });
    cursor.IncrementAndGet(1L);
    strategy.SignalAllWhenBlocking();
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    sequence_2.IncrementAndGet(1L);
    EXPECT_EQ(return_value.load(),kInitialCursorValue);

    alerted.store(true);
    waiter.join();
    EXPECT_EQ(return_value.load(),kAlertedSignal);
}

} // end namespace test
} // end namespace disruptor

#endif