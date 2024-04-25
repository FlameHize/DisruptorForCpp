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

    // get processor barrier without dependents
    std::vector<Sequence*> dependents;
    SequenceBarrier* barrier = sequencer->NewBarrier(dependents);

    // construct event processor with event_handler and above
    test::StubEventHandler event_handler;
    EventProcessor<test::StubEvent> event_processor(sequencer,barrier,&event_handler);
    std::thread consumer([&event_processor](){
        event_processor.Run();
    });

    // test add consumer gatting prevent producer wrap
    std::vector<Sequence*> gatting_sequences;
    // Sequence s;
    // s.SetSequence(500);
    // gatting_sequences.push_back(&s);

    gatting_sequences.push_back(event_processor.GetSequence());
    sequencer->SetGatingSequences(gatting_sequences);

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
    while(event_processor.GetSequence()->GetSequence() < expect_sequence) {
        // wait
    }
    gettimeofday(&end_time,NULL);

    double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << "Unicast 1P-1C performance: ";
    std::cout << (iterations * 1.0) / (end - start)
              << " ops/secs" << std::endl;
    std::cout << iterations * 64.0 / ((end - start) * 1000000)
              << " Mb/secs" << std::endl; 
    std::cout << (end - start) * 1000000000.0 / iterations
              << " latency/ns" << std::endl;
    event_processor.Stop();
    consumer.join();
    return 0;
}