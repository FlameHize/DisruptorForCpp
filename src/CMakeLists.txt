add_library(disruptor SHARED
        ring_buffer.cc
        sequence.cc
        wait_strategy.cc
        sequence_barrier.cc
        claim_strategy.cc
        sequencer.cc
        event/event_interface.cc
        event/event_producer.cc
        event/event_processor.cc
        )