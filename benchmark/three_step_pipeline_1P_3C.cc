#include "sequencer.h"
#include "event/event_producer.h"
#include "event/event_processor.h"
#include "support/stub_event.h"

#include <iostream>
#include <thread>
#include "sys/time.h"

using namespace disruptor;

int main(int argc,char** argv)
{
    // construct sequencer
    const int64_t ring_buffer_size = 1024 * 1024 * 64;
    Sequencer<test::StubEvent>* sequencer = new Sequencer<test::StubEvent>(ring_buffer_size,
                    kSingleThreadClaimStrategy,kBusySpinStrategy);
    
    // get consumer barrier with dependents
    std::vector<Sequence*> dependents;

    // event handler
    test::StubEventHandler event_handler;

    // first pipeline's barrier
    SequenceBarrier* first_barrier = sequencer->NewBarrier(dependents);
    EventProcessor<test::StubEvent> first_event_processor(sequencer,first_barrier,&event_handler);
    std::thread first_consumer([&first_event_processor](){
        first_event_processor.Run();
    });

    // second pipeline's barrier
    dependents.clear();
    dependents.push_back(first_event_processor.GetSequence());
    SequenceBarrier* second_barrier = sequencer->NewBarrier(dependents);
    EventProcessor<test::StubEvent> second_event_processor(sequencer,second_barrier,&event_handler);
    std::thread second_consumer([&second_event_processor](){
        second_event_processor.Run();
    });

    // third pipeline's barrier
    dependents.clear();
    dependents.push_back(second_event_processor.GetSequence());
    SequenceBarrier* third_barrier = sequencer->NewBarrier(dependents);
    EventProcessor<test::StubEvent> third_event_processor(sequencer,third_barrier,&event_handler);
    std::thread third_consumer([&third_event_processor](){
        third_event_processor.Run();
    });

    // add gating sequences for finally consumers
    std::vector<Sequence*> gating_sequences;
    gating_sequences.push_back(third_event_processor.GetSequence());
    sequencer->SetGatingSequences(gating_sequences);

    // construct event producer
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);

    test::StubEventTranslator event_translator;
    EventProducer<test::StubEvent> event_producer(sequencer);
    int64_t iterations = 500000000;
    int64_t batch_size = 1;
    for(int64_t i = 0; i < iterations; ++i) {
        event_producer.PublishEvent(&event_translator,batch_size);
    }

    int64_t expect_sequence = sequencer->GetCursor();
    while(third_event_processor.GetSequence()->GetSequence() < expect_sequence) {
        // wait
    }
    gettimeofday(&end_time,NULL);

    double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(12);
    std::cout << "Three_step_pipeline 1P-3C performance: " << std::endl;
    std::cout << "  Ops/secs: " 
              << (iterations * 1.0) / (end - start)
              << std::endl;
    std::cout << "  Mb/secs: " 
              << iterations * 64.0 / ((end - start) * 1000000)
              << std::endl; 
    std::cout << "  Latency/ns: "
              << (end - start) * 1000000000.0 / iterations
              << std::endl;

    first_event_processor.Stop();
    second_event_processor.Stop();
    third_event_processor.Stop();
    first_consumer.join();
    second_consumer.join();
    third_consumer.join();
}