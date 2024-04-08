add_library(disruptor SHARED
        ring_buffer.cc
        sequence.cc
        wait_strategy.cc
        consumer_barrier.cc
        claim_strategy.cc
        sequencer.cc
        event/event_interface.cc
        event/event_publisher.cc
        event/event_consumer.cc
        )