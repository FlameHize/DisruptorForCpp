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

#ifndef DISRUPTOR_EVENT_PUBLISHER_H_
#define DISRUPTOR_EVENT_PUBLISHER_H_

#include "sequencer.h"
#include "event/event_interface.h"

namespace disruptor {
template<typename T>
class EventPublisher
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(EventPublisher);
public:
    explicit EventPublisher(Sequencer<T>* sequencer)
        : _sequencer(sequencer) {}

    // Three statage
    // Call Next() get a sequence,then translator data into ringbuffer
    // and finally publish the event
    void PublishEvent(EventTranslator<T>* translator) {
        int64_t sequence = _sequencer->Next();
        translator->TranslateTo(sequence,_sequencer[sequence]);
        _sequencer->Publish(sequence);
    }

private:
    Sequencer<T>* _sequencer;
};
} // end namespace disruptor

#endif