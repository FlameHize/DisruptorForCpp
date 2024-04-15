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
    
    // get consumer barrier with dependents
    std::vector<Sequence*> dependents;

    // event handler
    test::StubEventHandler event_handler;

    // first pipeline's barrier
    ConsumerBarrier* first_barrier = sequencer->NewBarrier(dependents);
    EventConsumer<test::StubEvent> first_event_consumer(sequencer,first_barrier,&event_handler);
    std::thread first_consumer([&first_event_consumer](){
        first_event_consumer.Run();
    });

    // second pipeline's barrier
    dependents.clear();
    dependents.push_back(first_event_consumer.GetSequence());
    ConsumerBarrier* second_barrier = sequencer->NewBarrier(dependents);
    EventConsumer<test::StubEvent> second_event_consumer(sequencer,second_barrier,&event_handler);
    std::thread second_consumer([&second_event_consumer](){
        second_event_consumer.Run();
    });

    // third pipeline's barrier
    dependents.clear();
    dependents.push_back(second_event_consumer.GetSequence());
    ConsumerBarrier* third_barrier = sequencer->NewBarrier(dependents);
    EventConsumer<test::StubEvent> third_event_consumer(sequencer,third_barrier,&event_handler);
    std::thread third_consumer([&third_event_consumer](){
        third_event_consumer.Run();
    });

    // construct event publisher
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time,NULL);

    test::StubEventTranslator event_translator;
    EventPublisher<test::StubEvent> publisher(sequencer);
    int64_t iterations = 50000000;
    int64_t batch_size = 1;
    for(int64_t i = 0; i < iterations; ++i) {
        publisher.PublishEvent(&event_translator,batch_size);
    }

    int64_t expect_sequence = sequencer->GetCursor();
    while(third_event_consumer.GetSequence()->GetSequence() < expect_sequence) {
        // wait
    }
    gettimeofday(&end_time,NULL);

    double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << "Three_step_pipeline 1P-3C performance: ";
    std::cout << (iterations * 1.0) / (end - start)
              << " ops/secs" << std::endl;
    // std::cout << iterations * 64.0 / ((end - start) * 1000000)
    //           << " Mb/secs" << std::endl; 
    std::cout << (end - start) * 1000000000.0 / iterations
              << " latency/ns" << std::endl;

    first_event_consumer.Stop();
    second_event_consumer.Stop();
    third_event_consumer.Stop();
    first_consumer.join();
    second_consumer.join();
    third_consumer.join();
}