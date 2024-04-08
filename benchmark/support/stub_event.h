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

#ifndef DISRUPTOR_STUB_EVENT_H_
#define DISRUPTOR_STUB_EVENT_H_

#include "event/event_interface.h"

namespace disruptor {
namespace benchmark {

class StubEvent {
public:
    StubEvent(const int64_t& value = 0) : _value(value) {}

    int64_t GetValue() const { 
        return _value;
    }

    void SetValue(const int64_t& value) {
        _value = value;
    }

private:
    int64_t _value;
};

class StubEventFactory : public EventFactory<StubEvent> {
public:
    virtual StubEvent* NewInstance(const int64_t& size) const override {
        return new StubEvent[size];
    }
};

class StubEventHandler : public EventHandler<StubEvent> {
public:
    virtual void OnEvent(const int64_t& sequence,StubEvent& event) override {
        const int64_t sequence = event.GetValue();
    }
    virtual void OnStart() override {
        // empty temp
    }

    virtual void OnShutdown() override {
        // empty temp
    }
};

class StubEventTranslator : public EventTranslator<StubEvent> {
public:
    virtual StubEvent& TranslateTo(const int64_t& sequence,StubEvent& event) override {
        event.SetValue(sequence);
        return event;
    }
};

} // end namespace benchmark
} // end namespace disruptor

#endif