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

///@todo 五种测试模块 分别测试对应的wait strategy以及claim strategy
class EventTest : public testing::Test
{
public:
    int64_t ring_buffer_size = 8;

    Sequencer<StubEvent>* sequencer;
    SequenceBarrier* barrier;
    std::vector<Sequence*> dependents;
    
    StubEventTranslator event_translator;
    StubEventHandler event_handler;

    EventProcessor<StubEvent>* first_event_processor;
    EventProcessor<StubEvent>* second_event_processor;
    EventProcessor<StubEvent>* third_event_processor;

    EventProducer<StubEvent>* first_event_producer;
    EventProducer<StubEvent>* second_event_producer;
    EventProducer<StubEvent>* third_event_producer;
};

TEST_F(EventTest,Unicast1P1CWithWaitBusyStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    barrier = sequencer->NewBarrier(dependents);
    first_event_producer = new EventProducer<StubEvent>(sequencer);
    first_event_processor = new EventProcessor<StubEvent>
                    (sequencer,barrier,&event_handler);
    std::thread consumer([&](){
        first_event_processor->Run();
    });
    Sequence* processor_sequence = first_event_processor->GetSequence();

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
    EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + ring_buffer_size);

    first_event_processor->Stop();
    consumer.join();
}

TEST_F(EventTest,Unicast1P1CWithWaitSleepingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kSleepingStrategy);
    barrier = sequencer->NewBarrier(dependents);
    first_event_producer = new EventProducer<StubEvent>(sequencer);
    first_event_processor = new EventProcessor<StubEvent>
                    (sequencer,barrier,&event_handler);
    std::thread consumer([&](){
        first_event_processor->Run();
    });
    Sequence* processor_sequence = first_event_processor->GetSequence();

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
    EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + ring_buffer_size);

    first_event_processor->Stop();
    consumer.join();
}

TEST_F(EventTest,Unicast1P1CWithWaitYieldingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kYieldingStrategy);
    barrier = sequencer->NewBarrier(dependents);
    first_event_producer = new EventProducer<StubEvent>(sequencer);
    first_event_processor = new EventProcessor<StubEvent>
                    (sequencer,barrier,&event_handler);
    std::thread consumer([&](){
        first_event_processor->Run();
    });
    Sequence* processor_sequence = first_event_processor->GetSequence();

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
    EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + ring_buffer_size);

    first_event_processor->Stop();
    consumer.join();
}

///@bug can not return 概率性事件
// event_processor::Run()中 consumer线程处理事件 且此时run设置为false时
// BlockingStrategy才会停止
// 也就是说 虽然Stop()设置了alert,BlockingStrategy::WaitFor()阻塞在
// condition.wait()方法中 需要进行唤醒
TEST_F(EventTest,Unicast1P1CWithWaitBlockingStrategy)
{
    sequencer = new Sequencer<StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBlockingStrategy);
    barrier = sequencer->NewBarrier(dependents);
    first_event_producer = new EventProducer<StubEvent>(sequencer);
    first_event_processor = new EventProcessor<StubEvent>
                    (sequencer,barrier,&event_handler);
    std::thread consumer([&](){
        first_event_processor->Run();
    });
    Sequence* processor_sequence = first_event_processor->GetSequence();

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
    EXPECT_EQ(processor_sequence->GetSequence(),kFirstSequenceValue + ring_buffer_size);

    first_event_processor->Stop();

    // 进行一次wait_strategy.signal()通知 唤醒condition.wait() 判断alerted 
    // 结束循环
    first_event_producer->PublishEvent(&event_translator,0);

    consumer.join();
}

} // end namespace test

} // end namespace disruptor

#endif