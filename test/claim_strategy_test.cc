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

#ifndef DISRUPTOR_CLAIM_STRATEGY_TEST_H_
#define DISRUPTOR_CLAIM_STRATEGY_TEST_H_

#include <vector>
#include <gtest/gtest.h>
#include "claim_strategy.h"

#define RING_BUFFER_SIZE 8

namespace disruptor {
namespace test {

class ClaimStrategyTest: public testing::Test
{
public:
    Sequence cursor;
    Sequence sequence_1;
    Sequence sequence_2;
    Sequence sequence_3;
    std::vector<Sequence*> empty_dependents;
    ClaimStrategy* strategy;

    std::vector<Sequence*> OneDependents() {
        std::vector<Sequence*> result;
        result.push_back(&sequence_1);
        return result;
    }

    std::vector<Sequence*> AllDependents() {
        std::vector<Sequence*> result;
        result.push_back(&sequence_1);
        result.push_back(&sequence_2);
        result.push_back(&sequence_3);
        return result;
    }
};

class SingleClaimStrategyTest : public ClaimStrategyTest
{
    virtual void SetUp() override {
        strategy = CreateClaimStrategy(kSingleThreadClaimStrategy,RING_BUFFER_SIZE,cursor);
    }
};

TEST_F(SingleClaimStrategyTest,SingleIncrementAndGet)
{
    int64_t return_value = strategy->IncrementAndGet(empty_dependents);
    EXPECT_EQ(return_value,kFirstSequenceValue);

    int64_t delta = 100L;
    return_value = strategy->IncrementAndGet(empty_dependents,delta);
    EXPECT_EQ(return_value,kFirstSequenceValue + delta);
}

TEST_F(SingleClaimStrategyTest,SingleHasAvailableCapacity)
{
    auto one_dependents = OneDependents();
    int64_t return_value = strategy->IncrementAndGet(one_dependents,RING_BUFFER_SIZE);
    EXPECT_EQ(return_value,kInitialCursorValue + RING_BUFFER_SIZE);
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),false);

    // advance once latest consumers
    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),true);

    // only one slot free
    EXPECT_EQ(strategy->IncrementAndGet(one_dependents),return_value + 1L);
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),false);

    // dependents(consumers) keeps up
    sequence_1.IncrementAndGet(RING_BUFFER_SIZE); 
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),true);

    // all equals
    EXPECT_EQ(strategy->IncrementAndGet(one_dependents,RING_BUFFER_SIZE),
                sequence_1.IncrementAndGet(RING_BUFFER_SIZE));
}

class MultiClaimStrategyTest : public ClaimStrategyTest
{
    virtual void SetUp() override {
        strategy = CreateClaimStrategy(kMultiThreadClaimStrategy,RING_BUFFER_SIZE,cursor);
    }
};

TEST_F(MultiClaimStrategyTest,MultiIncrementAndGet)
{
    std::atomic<int64_t> return_value_1 = {kInitialCursorValue};
    std::atomic<int64_t> return_value_2 = {kInitialCursorValue};
    std::atomic<int64_t> return_value_3 = {kInitialCursorValue};

    return_value_1 = strategy->IncrementAndGet(empty_dependents);
    return_value_2 = strategy->IncrementAndGet(empty_dependents);
    return_value_3 = strategy->IncrementAndGet(empty_dependents);

    EXPECT_EQ(return_value_1,kFirstSequenceValue);
    EXPECT_EQ(return_value_2,kFirstSequenceValue + 1L);
    EXPECT_EQ(return_value_3,kFirstSequenceValue + 2L);
    EXPECT_EQ(strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1),kInitialCursorValue);

    strategy->Publish(return_value_1);
    EXPECT_EQ(strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1),return_value_1);

    strategy->Publish(return_value_3);
    EXPECT_EQ(strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1),return_value_1);

    strategy->Publish(return_value_2);
    EXPECT_EQ(strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1),return_value_3);

}

TEST_F(MultiClaimStrategyTest,MultiHasAvailableCapacity)
{
    auto one_dependents = OneDependents();
    int64_t return_value = strategy->IncrementAndGet(one_dependents,RING_BUFFER_SIZE);
    EXPECT_EQ(return_value,kInitialCursorValue + RING_BUFFER_SIZE);
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),false);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(strategy->HasAvailableCapacity(one_dependents),true);
    EXPECT_EQ(strategy->IncrementAndGet(one_dependents),return_value + 1L);

    // After this,publish.sequence == consumer.sequence
    sequence_1.IncrementAndGet(RING_BUFFER_SIZE);
    EXPECT_EQ(strategy->IncrementAndGet(one_dependents,RING_BUFFER_SIZE),
                    sequence_1.IncrementAndGet(RING_BUFFER_SIZE));
}

TEST_F(MultiClaimStrategyTest,SynchronizePublishingShouldBlockEagerThreads)
{
    std::atomic<bool> wait(true);
    std::atomic<bool> running_1(false);
    std::atomic<bool> running_2(false);
    std::atomic<bool> running_3(false);

    int64_t published;

    Sequence cursor;
    Sequence sequence_1;
    Sequence sequence_2;
    Sequence sequence_3;

    std::thread publisher_1([this,&wait,&cursor,&sequence_1,&running_1](){
        while(wait) {
        }
        sequence_1.SetSequence(strategy->IncrementAndGet(empty_dependents));
        cursor.SetSequence(sequence_1.GetSequence());
        while(!wait.load()) {
        };
        strategy->Publish(sequence_1.GetSequence());
        running_1.store(true);
    });

    std::thread publisher_2([this,&wait,&cursor,&sequence_2,&running_2](){
        while(wait) {
        }
        sequence_2.SetSequence(strategy->IncrementAndGet(empty_dependents));
        cursor.SetSequence(sequence_2.GetSequence());
        while(!wait.load()) {
        };
        strategy->Publish(sequence_2.GetSequence());
        running_2.store(true);
    });

    std::thread publisher_3([this,&wait,&cursor,&sequence_3,&running_3](){
        while(wait) {
        }
        sequence_3.SetSequence(strategy->IncrementAndGet(empty_dependents));
        cursor.SetSequence(sequence_3.GetSequence());
        while(!wait.load()) {
        };
        strategy->Publish(sequence_3.GetSequence());
        running_3.store(true);
    });

    wait.store(false);
    while(cursor.GetSequence() < kInitialCursorValue + 3L) {
        // wait
    }
    EXPECT_EQ(cursor.GetSequence(),kInitialCursorValue + 3L);
    EXPECT_EQ(strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1),kInitialCursorValue);

    wait.store(true);
    // while(running_1.load() && running_2.load() && running_3.load()) {
    //     // wait
    // }
    publisher_1.join();
    publisher_2.join();
    publisher_3.join();
    ///@bug 需要等待一段时间才会返回正确结果 否则始终返回-1
    // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_EQ(strategy->IsAvailable(kFirstSequenceValue),true);
    EXPECT_EQ(strategy->IsAvailable(kFirstSequenceValue + 1L),true);
    EXPECT_EQ(strategy->IsAvailable(kFirstSequenceValue + 2L),true);
    EXPECT_EQ(strategy->IsAvailable(kFirstSequenceValue + 3L),false);
    published = strategy->GetHighesetPublishedSequence(kFirstSequenceValue,RING_BUFFER_SIZE - 1);
    EXPECT_EQ(published,kInitialCursorValue + 3L);
}

}   // end namespace test
}   // end namespace disruptor

#endif