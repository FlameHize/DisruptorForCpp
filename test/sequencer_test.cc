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

#ifndef DISRUPTOR_SEQUENCER_TEST_H_
#define DISRUPTOR_SEQUENCER_TEST_H_

#include <gtest/gtest.h>
#include "sequencer.h"

#define RING_BUFFER_SIZE 4

namespace disruptor {
namespace test {

class SequencerTest : public testing::Test
{
public:
    SequencerTest() : sequencer(RING_BUFFER_SIZE,kSingleThreadClaimStrategy,kBusySpinStrategy) {
            std::vector<Sequence*> sequences;
            sequences.push_back(&gating_sequence);
            sequencer.SetGatingSequences(sequences);
        }

    void FillBuffer() {
        for(int i = 0; i < RING_BUFFER_SIZE; ++i) {
            int64_t sequence = sequencer.Next();
            sequencer.Publish(sequence);
        }
    }

    Sequencer<int64_t> sequencer;
    Sequence gating_sequence;
};

TEST_F(SequencerTest,StartWithValueInitialized) 
{
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue);
}

TEST_F(SequencerTest,UseOperatorGetSequencerValue)
{
    int64_t sequence = sequencer.Next();
    sequencer.Publish(sequence);
    *sequencer[sequence] = kInitialCursorValue;
    EXPECT_EQ(*sequencer[sequence],kInitialCursorValue);
}

TEST_F(SequencerTest,GetFirstPublishSequence)
{
    const int64_t sequence = sequencer.Next();
    EXPECT_EQ(sequence,kFirstSequenceValue);
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue);

    sequencer.Publish(sequence);
    EXPECT_EQ(sequencer.GetCursor(),sequence);
}

TEST_F(SequencerTest,IndicateAvailableCapacity)
{
    EXPECT_EQ(sequencer.HasAvailableCapacity(),true);
    FillBuffer();
    EXPECT_EQ(sequencer.HasAvailableCapacity(),false);
}

TEST_F(SequencerTest,GattingSequences)
{
    // The order in which SequencerBarrier is used to protect consumers
    // GattingSequences are used to prevent producers from breaking consumer data
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer.NewBarrier(dependents);
    FillBuffer();
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue + RING_BUFFER_SIZE);
    gating_sequence.SetSequence(kInitialCursorValue);
    std::atomic<bool> complete(true);
    std::thread producer([&](){
        complete.store(false);
        sequencer.Publish(sequencer.Next());
        complete.store(true);
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(1L));
    EXPECT_EQ(complete.load(),false);
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue + RING_BUFFER_SIZE);
    
    // it means proceesor complete process first event
    gating_sequence.SetSequence(kFirstSequenceValue);
    while(!complete.load()) {
    }
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue + RING_BUFFER_SIZE + 1L);
    producer.join();
}

TEST_F(SequencerTest,PublishSequenceBatch)
{
    const int64_t batch_size = 3;
    sequencer.Next(batch_size);
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue);
    sequencer.Publish(kInitialCursorValue + batch_size,batch_size);
    EXPECT_EQ(sequencer.GetCursor(),kInitialCursorValue + batch_size);
}

TEST_F(SequencerTest,WaitOnSequence)
{
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer.NewBarrier(dependents);
    const int64_t sequence = sequencer.Next();
    sequencer.Publish(sequence);
    EXPECT_EQ(barrier->WaitFor(sequence),sequence);
}

TEST_F(SequencerTest,WaitOnBatchSequence)
{
    // Batch read
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer.NewBarrier(dependents);
    sequencer.Publish(sequencer.Next());
    sequencer.Publish(sequencer.Next());
    const int64_t sequence = sequencer.Next();
    sequencer.Publish(sequence);
    EXPECT_EQ(barrier->WaitFor(kFirstSequenceValue),sequence);
}

TEST_F(SequencerTest,SignalWaitingSequencesWhenPublish)
{
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer.NewBarrier(dependents);

    std::atomic<bool> waiting(true);
    std::atomic<bool> completed(false);

    std::thread thread([&](){
        waiting.store(false);
        EXPECT_EQ(barrier->WaitFor(kFirstSequenceValue),kFirstSequenceValue);
        gating_sequence.SetSequence(kFirstSequenceValue);
        completed.store(true);
    });

    while(waiting.load()) {
    }
    EXPECT_EQ(gating_sequence.GetSequence(),kInitialCursorValue);

    sequencer.Publish(sequencer.Next());
    
    while(!completed.load()) {
    }
    EXPECT_EQ(gating_sequence.GetSequence(),kFirstSequenceValue);
    thread.join();
}

TEST_F(SequencerTest,BlockPublisherWhenRingBufferIsFull)
{
    std::atomic<bool> waiting(true);
    std::atomic<bool> completed(false);
    FillBuffer();
    const int64_t expected_full_cursor = kInitialCursorValue + RING_BUFFER_SIZE;
    EXPECT_EQ(sequencer.GetCursor(),expected_full_cursor);

    std::thread thread([&](){
        waiting.store(false);
        sequencer.Publish(sequencer.Next());
        completed.store(true);
    });
    while(waiting.load()) {
    }
    EXPECT_EQ(sequencer.GetCursor(),expected_full_cursor);

    gating_sequence.SetSequence(kInitialCursorValue + 1L);
    
    while(!completed.load()){
    }
    EXPECT_EQ(sequencer.GetCursor(),expected_full_cursor + 1L);
    thread.join();
}

} // end namespace test
} // end namespace disruptor

#endif