#include "sequencer.h"
#include "event/event_publisher.h"
#include "event/event_consumer.h"
#include "support/stub_event.h"

#include <iostream>
#include "sys/time.h"

using namespace disruptor;

int main(int* argc,char** argv) 
{
    // const size_t ring_buffer_size = 1024 * 8;
    // std::array<benchmark::StubEvent,kDefaultRingBufferSize> ring_buffer;
    // Sequencer<benchmark::StubEvent,ring_buffer_size,
    //     SingleThreadStrategy<ring_buffer_size>,BusySpinStrategy> sequencer(ring_buffer);
    
    // std::vector<Sequence*> dependents;
    // ConsumerBarrier<BusySpinStrategy>* barrier = sequencer.NewBarrier(dependents);
    // benchmark::StubEventHandler event_handler;
    // EventConsumer<benchmark::StubEvent,BusySpinStrategy> 
    //     event_consumer(&sequencer,barrier,&event_handler) event_consumer;

    // ///@bug 模板参数不匹配 
    // std::array<benchmark::StubEvent,kDefaultRingBufferSize> ring_buffer;
    // Sequencer<benchmark::StubEvent> sequencer(ring_buffer);

    // std::vector<Sequence*> dependents;
    // ConsumerBarrier<kDefaultWaitStrategy>* barrier = sequencer.NewBarrier(dependents);
    // benchmark::StubEventHandler event_handler;
    // EventConsumer<benchmark::StubEvent,kDefaultWaitStrategy> 
    //     event_consumer(&sequencer,barrier,&event_handler);

    // std::thread thread([&](){
    //     event_consumer.Run();
    // });

    // struct timeval start_time;
    // struct timeval end_time;
    // gettimeofday(&start_time,NULL);

    // benchmark::StubEventTranslator event_translator;
    // EventPublisher<benchmark::StubEvent> publisher(&sequencer);
    // int64_t iterations = 10000;
    // for(int64_t i = 0; i < iterations; ++i) {
    //     publisher.PublishEvent(&event_translator);
    // }

    // int64_t expect_sequence = sequencer.GetCursor();
    // while(event_consumer.GetSequence()->GetSequence() < expect_sequence) {
    //     // wait
    // }
    // gettimeofday(&end_time,NULL);

    // double start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    // double end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    // std::cout.precision(15);
    // std::cout << "1P-1EP-UNICAST performance: ";
    // std::cout << (iterations * 1.0) / (end - start)
    //           << " ops/secs" << std::endl;
    // event_consumer.Stop();
    // thread.join();
    return 0;
}