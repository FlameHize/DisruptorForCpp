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

template<typename Strategy>
class ClaimStrategyTest: public testing::Test
{
public:
    Sequence cursor;
    Sequence sequence_1;
    Sequence sequence_2;
    Sequence sequence_3;
    std::vector<Sequence*> empty_dependents;
    Strategy strategy;

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

using SingleClaimStrategyTest = ClaimStrategyTest<SingleThreadStrategy<RING_BUFFER_SIZE>>;

TEST_F(SingleClaimStrategyTest,SingleIncrementAndGet)
{
    int64_t return_value = strategy.IncrementAndGet(empty_dependents);
    EXPECT_EQ(return_value,kFirstSequenceValue);

    int64_t delta = 100L;
    return_value = strategy.IncrementAndGet(empty_dependents,delta);
    EXPECT_EQ(return_value,kFirstSequenceValue + delta);
}

TEST_F(SingleClaimStrategyTest,SingleHasAvailableCapacity)
{
    auto one_dependents = OneDependents();
    int64_t return_value = strategy.IncrementAndGet(one_dependents,RING_BUFFER_SIZE);
    EXPECT_EQ(return_value,kInitialCursorValue + RING_BUFFER_SIZE);
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),false);

    // advance once latest consumers
    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),true);

    // only one slot free
    EXPECT_EQ(strategy.IncrementAndGet(one_dependents),return_value + 1L);
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),false);

    // dependents(consumers) keeps up
    sequence_1.IncrementAndGet(RING_BUFFER_SIZE); 
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),true);

    // all equals
    EXPECT_EQ(strategy.IncrementAndGet(one_dependents,RING_BUFFER_SIZE),
                sequence_1.IncrementAndGet(RING_BUFFER_SIZE));
}

using MultiClaimStrategyTest = ClaimStrategyTest<MultiThreadStrategy<RING_BUFFER_SIZE>>;

TEST_F(MultiClaimStrategyTest,MultiIncrementAndGet)
{
    std::atomic<int64_t> return_value_1 = {kInitialCursorValue};
    std::atomic<int64_t> return_value_2 = {kInitialCursorValue};
    std::atomic<bool> wait_1 = {true};
    std::atomic<bool> wait_2 = {true};

    std::thread publisher_1([this,&return_value_1,&wait_1](){
        while(wait_1) {
        };
        return_value_1 = strategy.IncrementAndGet(empty_dependents);
    });

    std::thread publisher_2([this,&return_value_2,&wait_2](){
        while(wait_2) {
        };
        return_value_2 = strategy.IncrementAndGet(empty_dependents);
    });

    // return_value_1 to be kInitialCursorValue + 1L
    wait_1.store(false);
    publisher_1.join();

    // return_value_2 to be kInitialCursorValue + 2L
    wait_2.store(false);
    publisher_2.join();

    EXPECT_EQ(return_value_1,kFirstSequenceValue);
    EXPECT_EQ(return_value_2,kFirstSequenceValue + 1L);
}

TEST_F(MultiClaimStrategyTest,MultiHasAvailableCapacity)
{
    auto one_dependents = OneDependents();
    int64_t return_value = strategy.IncrementAndGet(one_dependents,RING_BUFFER_SIZE);
    EXPECT_EQ(return_value,kInitialCursorValue + RING_BUFFER_SIZE);
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),false);

    sequence_1.IncrementAndGet(1L);
    EXPECT_EQ(strategy.HasAvailableCapacity(one_dependents),true);
    EXPECT_EQ(strategy.IncrementAndGet(one_dependents),return_value + 1L);

    // After this,publish.sequence == consumer.sequence
    sequence_1.IncrementAndGet(RING_BUFFER_SIZE);
    EXPECT_EQ(strategy.IncrementAndGet(one_dependents,RING_BUFFER_SIZE),
                    sequence_1.IncrementAndGet(RING_BUFFER_SIZE));
}

TEST_F(MultiClaimStrategyTest,SynchronizePublishingShouldBlockEagerThreads)
{
    std::atomic<bool> running_1(true);
    std::atomic<bool> wait_1(true);
    Sequence claimed_1;
    
    std::atomic<bool> running_2(true);
    std::atomic<bool> wait_2(true);
    Sequence claimed_2;

    std::atomic<bool> running_3(true);
    std::atomic<bool> wait_3(true);
    Sequence claimed_3;

    Sequence cursor;

    std::thread publisher_1([this,&running_1,&wait_1,&claimed_1,&cursor](){
        while(wait_1) {
        };
        // claimed_1 to be kFirstSequenceValue
        claimed_1.SetSequence(strategy.IncrementAndGet(empty_dependents));
        wait_1.store(true);
        while(wait_1){
        };
        // kFirstSequenceValue is the maxium sequence to be published
        strategy.SynchronizePublishing(kFirstSequenceValue,cursor,1);
        running_1.store(false);
    });

    std::thread publisher_2([this,&running_2,&wait_2,&claimed_2,&cursor](){
        while(wait_2) {
        };
        // claimed_2 to be kFirstSequenceValue + 1L
        claimed_2.SetSequence(strategy.IncrementAndGet(empty_dependents));
        wait_2.store(true);
        while(wait_2){
        };
        // kFirstSequenceValue + 1L is the maxium sequence to be published
        strategy.SynchronizePublishing(kFirstSequenceValue + 1L,cursor,1);
        running_2.store(false);
    });

    std::thread publisher_3([this,&running_3,&wait_3,&claimed_3,&cursor](){
        while(wait_3) {
        };
        // claimed_3 to be kFirstSequenceValue + 2L
        claimed_3.SetSequence(strategy.IncrementAndGet(empty_dependents));
        wait_3.store(true);
        while(wait_3){
        };
        // kFirstSequenceValue + 2L is the maxium sequence to be published
        strategy.SynchronizePublishing(kFirstSequenceValue + 2L,cursor,1);
        running_3.store(false);
    });

    // publisher_1 claims but not published
    wait_1.store(false);
    while(!wait_1.load()){
    };
    EXPECT_EQ(claimed_1.GetSequence(),kFirstSequenceValue);
    EXPECT_EQ(claimed_2.GetSequence(),kInitialCursorValue);
    EXPECT_EQ(claimed_3.GetSequence(),kInitialCursorValue);

    // publisher_2 claims but not published
    wait_2.store(false);
    while(!wait_2.load()){
    };
    EXPECT_EQ(claimed_1.GetSequence(),kFirstSequenceValue);
    EXPECT_EQ(claimed_2.GetSequence(),kFirstSequenceValue + 1L);
    EXPECT_EQ(claimed_3.GetSequence(),kInitialCursorValue);

    // publisher_3 claims but not published
    wait_3.store(false);
    while(!wait_3.load()){
    };
    EXPECT_EQ(claimed_1.GetSequence(),kFirstSequenceValue);
    EXPECT_EQ(claimed_2.GetSequence(),kFirstSequenceValue + 1L);
    EXPECT_EQ(claimed_3.GetSequence(),kFirstSequenceValue + 2L);

    // publisher_2 and publisher_3 continue running but must wait on
    // publisher_1 to be publish
    wait_2.store(false);
    wait_3.store(false);
    EXPECT_EQ(running_2,true);
    EXPECT_EQ(running_3,true);

    // publisher_1 publish(not update cursor)
    wait_1.store(false);
    publisher_1.join();
    EXPECT_EQ(running_2,true);
    EXPECT_EQ(running_3,true);

    // mock update cursor to sync publisher_2
    cursor.IncrementAndGet(1L);

    // publisher_2 is now free but publisher_3 is still locked
    publisher_2.join();
    EXPECT_EQ(running_3,true);

    // mock update cursor once more to sync publisher_3
    // now publisher_3 is free
    cursor.IncrementAndGet(1L);
    publisher_3.join();
}

}   // end namespace test
}   // end namespace disruptor

#endif