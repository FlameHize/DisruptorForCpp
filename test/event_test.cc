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

#ifndef DISRUPTOR_EVENT_TEST_H_
#define DISRUPTOR_EVENT_TEST_H_

#include "sequencer.h"
#include "event/event_interface.h"
#include "event/event_producer.h"
#include "event/event_processor.h"
#include "support/stub_event.h"
#include <gtest/gtest.h>

namespace disruptor {
namespace test {

class EventTest : public testing::Test
{
public:
    int64_t ring_buffer_size = 8;

    Sequencer<StubEvent>* sequencer;
    SequenceBarrier* barrier;
    std::vector<Sequence*> dependents;
    
    StubEventTranslator event_translator;
    StubEventHandler event_handler;

    SequenceBarrier* first_barrier;
    SequenceBarrier* second_barrier;
    SequenceBarrier* third_barrier;

    EventProcessor<StubEvent>* first_event_processor;
    EventProcessor<StubEvent>* second_event_processor;
    EventProcessor<StubEvent>* third_event_processor;

    EventProducer<StubEvent>* first_event_producer;
    EventProducer<StubEvent>* second_event_producer;
    EventProducer<StubEvent>* third_event_producer;

    void Unicast1P1C() {
        barrier = sequencer->NewBarrier(dependents);
        first_event_producer = new EventProducer<StubEvent>(sequencer);
        first_event_processor = new EventProcessor<StubEvent>
                        (sequencer,barrier,&event_handler);
        std::thread consumer([&](){
            first_event_processor->Run();
        });

        Sequence* processor_sequence = first_event_processor->GetSequence();
        std::vector<Sequence*> gating_sequences;
        gating_sequences.push_back(processor_sequence);
        sequencer->SetGatingSequences(gating_sequences);

        first_event_producer->PublishEvent(&event_translator,1);
        int64_t expect_sequence = sequencer->GetCursor();
        while(processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue);

        first_event_producer->PublishEvent(&event_translator,3);
        expect_sequence = sequencer->GetCursor();
        while(processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        
        // full
        first_event_producer->PublishEvent(&event_translator,5);
        expect_sequence = sequencer->GetCursor();
        while(processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + 8L);

        first_event_producer->PublishEvent(&event_translator,8);
        expect_sequence = sequencer->GetCursor();
        while(processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + 16L);

        first_event_processor->Stop();
        consumer.join();
    }

    void Pipeline1P3C() {
        first_barrier = sequencer->NewBarrier(dependents);
        first_event_producer = new EventProducer<StubEvent>(sequencer);
        
        first_event_processor = new EventProcessor<StubEvent>
                        (sequencer,first_barrier,&event_handler);
        std::thread first_consumer([&](){
            first_event_processor->Run();
        });

        dependents.clear();
        dependents.push_back(first_event_processor->GetSequence());
        second_barrier = sequencer->NewBarrier(dependents);
        second_event_processor = new EventProcessor<StubEvent>
                        (sequencer,second_barrier,&event_handler);
        std::thread second_consumer([&](){
            second_event_processor->Run();
        });

        dependents.clear();
        dependents.push_back(second_event_processor->GetSequence());
        third_barrier = sequencer->NewBarrier(dependents);
        third_event_processor = new EventProcessor<StubEvent>
                        (sequencer,third_barrier,&event_handler);
        std::thread third_consumer([&](){
            third_event_processor->Run();
        });

        Sequence* first_processor_sequence = first_event_processor->GetSequence();
        Sequence* second_processor_sequence = second_event_processor->GetSequence();
        Sequence* third_processor_sequence = third_event_processor->GetSequence();

        std::vector<Sequence*> gating_sequences;
        gating_sequences.push_back(third_processor_sequence);
        sequencer->SetGatingSequences(gating_sequences);

        // first pipeline
        first_event_producer->PublishEvent(&event_translator,1);
        int64_t expect_sequence = sequencer->GetCursor();
        while(first_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue);

        // second pipeline
        first_event_producer->PublishEvent(&event_translator,3);
        expect_sequence = sequencer->GetCursor();
        while(second_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);

        // third pipeline
        first_event_producer->PublishEvent(&event_translator,5);
        expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);

        first_event_producer->PublishEvent(&event_translator,8);
        expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);

        first_event_processor->Stop();
        second_event_processor->Stop();
        third_event_processor->Stop();
        first_consumer.join();
        second_consumer.join();
        third_consumer.join();
    }

    void MultiCast1P3C() {
        first_event_producer = new EventProducer<StubEvent>(sequencer);
        barrier = sequencer->NewBarrier(dependents);

        first_event_processor = new EventProcessor<StubEvent>
                        (sequencer,barrier,&event_handler);
        std::thread first_consumer([&](){
            first_event_processor->Run();
        });

        second_event_processor = new EventProcessor<StubEvent>
                        (sequencer,barrier,&event_handler);
        std::thread second_consumer([&](){
            second_event_processor->Run();
        });

        third_event_processor = new EventProcessor<StubEvent>
                        (sequencer,barrier,&event_handler);
        std::thread third_consumer([&](){
            third_event_processor->Run();
        });

        Sequence* first_processor_sequence = first_event_processor->GetSequence();
        Sequence* second_processor_sequence = second_event_processor->GetSequence();
        Sequence* third_processor_sequence = third_event_processor->GetSequence();

        std::vector<Sequence*> gating_sequences;
        gating_sequences.push_back(first_processor_sequence);
        gating_sequences.push_back(second_processor_sequence);
        gating_sequences.push_back(third_processor_sequence);
        sequencer->SetGatingSequences(gating_sequences);

        first_event_producer->PublishEvent(&event_translator,1);
        int64_t expect_sequence = sequencer->GetCursor();
        while(first_processor_sequence->GetSequence() < expect_sequence ||
                second_processor_sequence->GetSequence() < expect_sequence ||
                    third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue);

        first_event_producer->PublishEvent(&event_translator,3);
        expect_sequence = sequencer->GetCursor();
        while(first_processor_sequence->GetSequence() < expect_sequence ||
                second_processor_sequence->GetSequence() < expect_sequence ||
                    third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);

        first_event_producer->PublishEvent(&event_translator,5);
        expect_sequence = sequencer->GetCursor();
        while(first_processor_sequence->GetSequence() < expect_sequence ||
                second_processor_sequence->GetSequence() < expect_sequence ||
                    third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);

        first_event_producer->PublishEvent(&event_translator,8);
        expect_sequence = sequencer->GetCursor();
        while(first_processor_sequence->GetSequence() < expect_sequence ||
                second_processor_sequence->GetSequence() < expect_sequence ||
                    third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);

        first_event_processor->Stop();
        second_event_processor->Stop();
        third_event_processor->Stop();
        first_consumer.join();
        second_consumer.join();
        third_consumer.join();
    }

    void Diamond1P3C() {
        first_event_producer = new EventProducer<StubEvent>(sequencer);
        dependents.clear();
        first_barrier = sequencer->NewBarrier(dependents);

        first_event_processor = new EventProcessor<StubEvent>
                        (sequencer,first_barrier,&event_handler);
        std::thread first_consumer([&](){
            first_event_processor->Run();
        });

        second_event_processor = new EventProcessor<StubEvent>
                        (sequencer,first_barrier,&event_handler);
        std::thread second_consumer([&](){
            second_event_processor->Run();
        });

        dependents.clear();
        dependents.push_back(first_event_processor->GetSequence());
        dependents.push_back(second_event_processor->GetSequence());
        second_barrier = sequencer->NewBarrier(dependents);
        third_event_processor = new EventProcessor<StubEvent>
                        (sequencer,second_barrier,&event_handler);
        std::thread third_consumer([&](){
            third_event_processor->Run();
        });

        Sequence* first_processor_sequence = first_event_processor->GetSequence();
        Sequence* second_processor_sequence = second_event_processor->GetSequence();
        Sequence* third_processor_sequence = third_event_processor->GetSequence();

        std::vector<Sequence*> gating_sequences;
        gating_sequences.push_back(third_processor_sequence);
        sequencer->SetGatingSequences(gating_sequences);

        first_event_producer->PublishEvent(&event_translator,1);
        int64_t expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        // fix sequence change before processor event bug
        // in EventProcessor::Run() next_sequence calculate
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue);

        first_event_producer->PublishEvent(&event_translator,3);
        expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 3L);

        first_event_producer->PublishEvent(&event_translator,5);
        expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 8L);

        first_event_producer->PublishEvent(&event_translator,8);
        expect_sequence = sequencer->GetCursor();
        while(third_processor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(first_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(second_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);
        EXPECT_EQ(third_processor_sequence->GetSequence(),kFirstSequenceValue + 16L);

        first_event_processor->Stop();
        second_event_processor->Stop();
        third_event_processor->Stop();
        first_consumer.join();
        second_consumer.join();
        third_consumer.join();
    }

    void Sequencer3P1C() {
        barrier = sequencer->NewBarrier(dependents);

        first_event_producer = new EventProducer<StubEvent>(sequencer);
        second_event_producer = new EventProducer<StubEvent>(sequencer);
        third_event_producer = new EventProducer<StubEvent>(sequencer);

        first_event_processor = new EventProcessor<StubEvent>
                        (sequencer,barrier,&event_handler);
        std::thread consumer([&](){
            first_event_processor->Run();
        });

        Sequence* proceesor_sequence = first_event_processor->GetSequence();
        std::vector<Sequence*> gating_sequences;
        gating_sequences.push_back(proceesor_sequence);
        sequencer->SetGatingSequences(gating_sequences);

        std::thread first_producer([&](){
            first_event_producer->PublishEvent(&event_translator,1); 
        });
        std::thread second_producer([&](){
            second_event_producer->PublishEvent(&event_translator,1);
        });
        std::thread third_producer([&](){
            third_event_producer->PublishEvent(&event_translator,1);
        });
        first_producer.join();
        second_producer.join();
        third_producer.join();

        int64_t expect_sequence = sequencer->GetCursor();
        while(proceesor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(proceesor_sequence->GetSequence(),kInitialCursorValue + 3L);

        std::thread first_producer_2([&](){
            first_event_producer->PublishEvent(&event_translator,1); 
        });
        std::thread second_producer_2([&](){
            second_event_producer->PublishEvent(&event_translator,3);
        });
        std::thread third_producer_2([&](){
            third_event_producer->PublishEvent(&event_translator,5);
        });
        first_producer_2.join();
        second_producer_2.join();
        third_producer_2.join();

        expect_sequence = sequencer->GetCursor();
        while(proceesor_sequence->GetSequence() < expect_sequence) {
            // wait
        }
        EXPECT_EQ(proceesor_sequence->GetSequence(),kInitialCursorValue + 12L);

        first_event_processor->Stop();
        consumer.join();
    }
};

TEST_F(EventTest,Unicast1P1CWithWaitBusyStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    Unicast1P1C();
}

TEST_F(EventTest,Unicast1P1CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kSleepingStrategy);
    Unicast1P1C();
}

TEST_F(EventTest,Unicast1P1CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kYieldingStrategy);
    Unicast1P1C();
}

TEST_F(EventTest,Unicast1P1CWithWaitBlockingStrategy)
{
    // add SequencerBarrier::SignalAllWhenBlocking function to signal
    // first_event_producer->PublishEvent(&event_translator,0);
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBlockingStrategy);
    Unicast1P1C();
}


TEST_F(EventTest,Pipeline1P3CWithWaitBusyStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    Pipeline1P3C();
}

TEST_F(EventTest,Pipeline1P3CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kSleepingStrategy);
    Pipeline1P3C();
}

TEST_F(EventTest,Pipeline1P3CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kYieldingStrategy);
    Pipeline1P3C();
}

TEST_F(EventTest,Pipeline1P3CWithWaitBlockingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBlockingStrategy);
    Pipeline1P3C();
}

TEST_F(EventTest,Multicast1P3CWithWaitBusySpinStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    MultiCast1P3C();
}

TEST_F(EventTest,Multicast1P3CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kSleepingStrategy);
    MultiCast1P3C();
}

TEST_F(EventTest,Multicast1P3CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kYieldingStrategy);
    MultiCast1P3C();
}

TEST_F(EventTest,Multicast1P3CWithWaitBlockingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBlockingStrategy);
    MultiCast1P3C();
}

TEST_F(EventTest,Diamond1P3CWithWaitBusySpinStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    Diamond1P3C();
}

TEST_F(EventTest,Diamond1P3CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kSleepingStrategy);
    Diamond1P3C();
}

TEST_F(EventTest,Diamond1P3CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kYieldingStrategy);
    Diamond1P3C();
}

TEST_F(EventTest,Diamond1P3CWithWaitBlockingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBlockingStrategy);
    Diamond1P3C();
}

TEST_F(EventTest,Sequencer3P1CWithWaitBusySpinStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kMultiThreadClaimStrategy,kBusySpinStrategy);
    Sequencer3P1C();
}

TEST_F(EventTest,Sequencer3P1CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kMultiThreadClaimStrategy,kSleepingStrategy);
    Sequencer3P1C();
}

TEST_F(EventTest,Sequencer3P1CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kMultiThreadClaimStrategy,kYieldingStrategy);
    Sequencer3P1C();
}

TEST_F(EventTest,Sequencer3P1CWithWaitBlockingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kMultiThreadClaimStrategy,kBlockingStrategy);
    Sequencer3P1C();
}


} // end namespace test

} // end namespace disruptor

#endif