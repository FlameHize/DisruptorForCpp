#include "sequencer.h"
#include "event/event_publisher.h"
#include "event/event_consumer.h"
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

    // get consumer barrier without dependents
    std::vector<Sequence*> dependents;
    ConsumerBarrier* barrier = sequencer->NewBarrier(dependents);

    // construct event consumer with event_handler and above
    test::StubEventHandler event_handler;
    EventConsumer<test::StubEvent> event_consumer(sequencer,barrier,&event_handler);
    std::thread consumer([&event_consumer](){
        event_consumer.Run();
    });

    // construct event publisher
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);

    test::StubEventTranslator event_translator;
    EventPublisher<test::StubEvent> publisher(sequencer);
    int64_t iterations = 10000000;
    for(int64_t i = 0; i < iterations; ++i) {
        publisher.PublishEvent(&event_translator);
    }

    int64_t expect_sequence = sequencer->GetCursor();
    while(event_consumer.GetSequence()->GetSequence() < expect_sequence) {
        // wait
    }
    gettimeofday(&end_time,NULL);

    double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << "1P-1EP-UNICAST performance: ";
    std::cout << (iterations * 1.0) / (end - start)
              << " ops/secs" << std::endl;
    // std::cout << iterations * 64.0 / ((end - start) * 1000000)
    //           << " Mb/secs" << std::endl; 
    std::cout << (end - start) * 1000000000.0 / iterations
              << " latency/ns" << std::endl;
    event_consumer.Stop();
    consumer.join();
    return 0;
}