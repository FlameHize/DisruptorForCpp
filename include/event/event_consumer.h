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

#ifndef DISRUPTOR_EVENT_CONSUMER_H_
#define DISRUPTOR_EVENT_CONSUMER_H_

#include "sequencer.h"
#include "event/event_interface.h"

namespace disruptor {

template<typename T,typename W>
class EventConsumer : public EventProcessor<T>
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(EventConsumer);
public:
    explicit EventConsumer(Sequencer<T>* sequencer,
                           ConsumerBarrier<W>* consumer_barrier,
                           EventHandler<T>* event_handler)
        : _running(false),
          _sequencer(sequencer),
          _consumer_barrier(consumer_barrier),
          _event_handler(event_handler) {}

    virtual Sequence* GetSequence() override {
        return &_sequence;
    }

    virtual void Run() override {
        if(_running.load()) {
            return;
        }
        _running.store(true);
        _consumer_barrier->SetAlerted(false);
        _event_handler->OnStart();
        
        // T* event = nullptr;
        int64_t next_sequence = _sequence.IncrementAndGet(1L);
        while(true) {
            int64_t available_sequence = _consumer_barrier->WaitFor(next_sequence);
            while(next_sequence <= available_sequence) {
                T& event = _sequencer[next_sequence];
                _event_handler->OnEvent(next_sequence,event);
                ++next_sequence;
            }
            _sequence.SetSequence(next_sequence - 1L);
            if(!_running.load()) {
                break;
            }
        }
        _event_handler->OnShutdown();
        _running.store(false);
    }

    virtual void Stop() override {
        if(!_running.load()) {
            return;
        }
        _running.store(false);
        _consumer_barrier->SetAlerted(true);
    }

private:
    std::atomic<bool> _running;
    Sequence _sequence;
    Sequencer<T>* _sequencer;
    ConsumerBarrier<W>* _consumer_barrier;
    EventHandler<T>* _event_handler;
};

} // end namespace disruptor

#endif