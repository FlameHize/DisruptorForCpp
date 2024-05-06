#include "sequencer.h"
#include "event/event_producer.h"
#include "event/event_processor.h"
#include "support/stub_event.h"

#include <iostream>
#include <thread>
#include "sys/time.h"

using namespace disruptor;

///@bug kMultiThreadClaimStrategy 
int main(int argc,char** argv)
{
    // construct sequencer
    const int64_t ring_buffer_size = 1024 * 1024 * 64;
    Sequencer<test::StubEvent>* sequencer = new Sequencer<test::StubEvent>(ring_buffer_size,
                    kMultiThreadClaimStrategy,kBusySpinStrategy);
    
    // get processor without dependents
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer->NewBarrier(dependents);

    // construct event processor with event handler
    test::StubEventHandler event_handler;
    EventProcessor<test::StubEvent> event_processor(sequencer,barrier,&event_handler);
    std::thread consumer([&event_processor](){
        event_processor.Run();
    });
    Sequence* proceesor_sequence = event_processor.GetSequence();
    std::vector<Sequence*> gating_sequences;
    gating_sequences.push_back(proceesor_sequence);
    sequencer->SetGatingSequences(gating_sequences);

    // construct event translator
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);

    test::StubEventTranslator event_translator;
    int64_t iterations = 500000000;
    int64_t batch_size = 1;

    // first event producer
    EventProducer<test::StubEvent> first_event_producer(sequencer);
    std::thread first_publisher([&](){
        for(int64_t i = 0; i < iterations; ++i) {
            first_event_producer.PublishEvent(&event_translator,batch_size);
        }
    });

    // second event producer
    EventProducer<test::StubEvent> second_event_producer(sequencer);
    std::thread second_publisher([&](){
        for(int64_t i = 0; i < iterations; ++i) {
            second_event_producer.PublishEvent(&event_translator,batch_size);
        }
    });

    // third event producer
    EventProducer<test::StubEvent> third_event_producer(sequencer);
    std::thread third_publisher([&](){
        for(int64_t i = 0; i < iterations; ++i) {
            third_event_producer.PublishEvent(&event_translator,batch_size);
        }
    });


    int64_t expect_sequence = sequencer->GetCursor();
    while(proceesor_sequence->GetSequence() < iterations * 3 - 1) {
        // wait
    }
    gettimeofday(&end_time,NULL);

    double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(12);
    std::cout << "Sequencer 3P-1C performance: " << std::endl;
    std::cout << "  Ops/secs: " 
              << (iterations * 1.0) / (end - start)
              << std::endl;
    std::cout << "  Mb/secs: " 
              << iterations * 64.0 / ((end - start) * 1000000)
              << std::endl; 
    std::cout << "  Latency/ns: "
              << (end - start) * 1000000000.0 / iterations
              << std::endl;

    event_processor.Stop();
    consumer.join();
    first_publisher.join();
    second_publisher.join();
    third_publisher.join();
    return 0; 
}