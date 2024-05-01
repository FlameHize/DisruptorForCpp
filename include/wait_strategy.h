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

#ifndef DISRUPTOR_WAIT_STRATEGY_H_
#define DISRUPTOR_WAIT_STRATEGY_H_

#include <sys/time.h>
#include <chrono>
#include <functional>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>

#include "sequence.h"
#include "utils.h"

namespace disruptor {

// Strategy options which are available to those waiting on a ringbuffer
enum WaitStrategyOption {
    // This strategy uses a condition variable inside a lock to block the
    // event procesor which saves CPU resource at the expense of lock
    // contention.
    kBlockingStrategy,
    // This strategy uses a progressive back off strategy by first spinning,
    // then yielding, then sleeping for 1ms period. This is a good strategy
    // for burst traffic then quiet periods when latency is not critical.
    kSleepingStrategy,
    // This strategy calls Thread.yield() in a loop as a waiting strategy
    // which reduces contention at the expense of CPU resource.
    kYieldingStrategy,
    // This strategy call spins in a loop as a waiting strategy which is
    // lowest and most consistent latency but ties up a CPU.
    kBusySpinStrategy
};

class WaitStrategy
{
public:
    /**
     * @brief Wait for the given sequence to be available for consumer
     * @param sequence to wait for
     * @param cursor sequencer's cursor
     * @param dependents Dependent consumers queue ex: app dependents log and database
     * @param alerted indicator of consumer alert
     * @return kAltertedSignal if the barrier signaled an alert, otherwise return 
     * the greatest available sequence which may be greater than request
    */
    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alarted) = 0;

    /**
     * @brief Wait for the given sequence to be available for consumption 
     * with a specified timeout.
     * @param sequence to wait for
     * @param cursor sequencer's cursor
     * @param dependents all wait's consumer's sequence queue
     * @param alerted indicator of consumer alert
     * @param timeout indicator of consumer wait time unit:us
     * @return kAltertedSignal if the barrier signaled an alert,
     * kTimeoutSignal if the the requested timeout was reached while
     * waiting, otherwise return the greatest available sequence which
     * may be higher than requested
    */
    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted,
                    const std::chrono::microseconds& timeout) = 0;

    /**
     * @brief Signal the strategy that the cursor as advanced. Some strategy depends
     * on this behaviour to unblock.
    */
    virtual void SignalAllWhenBlocking() = 0;
};

static inline WaitStrategy* CreateWaitStrategy(WaitStrategyOption option);

// Busy Spin strategy that uses a busy spin loop waiting on a barrier.
// This strategy will use CPU resource to avoid syscalls which can introduce
// latency jitter. It is strongly recommended to pin threads on isolated
// CPU cores to minimize context switching and latency.
class BusySpinStrategy;

// Yielding strategy that uses a thread::yield() for waiting on a barrier.
// This strategy is a good compromise between performance and CPU resource.
class YieldingStrategy;

// Sleeping strategy uses a progressive back off strategy by first spinning for
// S/2 loops, then yielding for S/2 loops, and finally sleeping for
// duration<D,DV> until ready to advance. This is a good strategy for burst
// traffic then quiet periods when latency is not critical.
class SleepingStrategy;

// Blocking strategy that waits for the sequencer's cursor to advance on the
// requested sequence.
//
// The sequencer MUST call SignalAllWhenBlocking() to unblock the strategy
// when waiting on the cursor.  Once the cursor is advanced, the strategy will
// busy spin on the dependents' sequences and can be cancelled by affecting the
// `alerted` atomic.
//
// The user can optionnaly provide a maximum timeout to the blocking operation,
// see std::condition_any::wait_for() documentation for limitations.
//
// This strategy uses a condition variable inside a lock(CAS) to block the
// event procesor which saves CPU resource at the expense of lock
// contention. Publishers must explicitely call SignalAllWhenBlocking()
// to unblock consumers. This strategy should be used when performance and
// low-latency are not as important as CPU resource.
class BlockingStrategy;

// default
using kDefaultWaitStrategy = BusySpinStrategy;
using kDefaultDuration = std::chrono::microseconds; 
constexpr int64_t kDefaultRetryLoops = 200L;
constexpr int kDefaultDurationValue = 1;

// used internally
// inline function allow multi define in file
static inline std::function<int64_t()> buildMinSequenceFunction(
    const Sequence& cursor, const std::vector<Sequence*>& dependents);

/**
 * @brief Strategy employed for a consumer to wait on the sequencer's cursor
*/
class BusySpinStrategy : public WaitStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(BusySpinStrategy);
public:
    explicit BusySpinStrategy() {}

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alarted) override {
        int64_t available_sequence = kInitialCursorValue;
        // If there is no dependents(consumers) min_sequence() is cursor.sequence
        // otherwise min_sequence() is minumum dependents's sequence
        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);
        while((available_sequence = min_sequence()) < sequence) {
            if(alarted.load()) {
                return kAlertedSignal;
            }
        }
        return available_sequence;
    }

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted,
                    const std::chrono::microseconds& timeout) override {
        int64_t available_value = kInitialCursorValue;

        const auto start = std::chrono::system_clock::now();
        const auto stop =  start + timeout;
        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);

        while((available_value = min_sequence()) < sequence) {
            if(alerted.load()) {
                return kAlertedSignal;
            }
            if(std::chrono::system_clock::now() >= stop) {
                return kTimeoutSignal;
            }
        }
        return available_value;
    }

    virtual void SignalAllWhenBlocking() override {}
};

class YieldingStrategy : public WaitStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(YieldingStrategy);
public:
    explicit YieldingStrategy(int64_t retry_loop = kDefaultRetryLoops) 
        : _retry_loop(retry_loop) {}

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted) override {
        int64_t available_sequence = kInitialCursorValue;
        int64_t counter = _retry_loop;
        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);
        while((available_sequence = min_sequence()) < sequence) {
            if(alerted.load()) {
                return kAlertedSignal;
            }
            counter = ApplyWaitMethod(counter);
        }
        return available_sequence;
    }

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted,
                    const std::chrono::microseconds& timeout) override {
        int64_t available_sequence = kInitialCursorValue;
        int64_t counter = _retry_loop;

        const auto start = std::chrono::system_clock::now();
        const auto stop = start + timeout;
        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);

        while((available_sequence = min_sequence()) < sequence) {
            if(alerted.load()) {
                return kAlertedSignal;
            }
            counter = ApplyWaitMethod(counter);
            if(stop <= std::chrono::system_clock::now()) {
                return kTimeoutSignal;
            }
        }
        return available_sequence;
    }

    virtual void SignalAllWhenBlocking() override {}

private:
    int64_t ApplyWaitMethod(int64_t counter) {
        // wait for setting thread loop
        if(counter) {
            return --counter;
        }
        // if counter is down,change thread to yield()
        std::this_thread::yield();
        return counter;
    }

    int64_t _retry_loop;
};

class SleepingStrategy : public WaitStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(SleepingStrategy);
public:
    explicit SleepingStrategy(int64_t retry_loop = kDefaultRetryLoops,
                              int64_t duration_value = kDefaultDurationValue) 
                : _retry_loop(retry_loop),
                  _duration_value(duration_value) {}

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted) override {
        int64_t available_sequence = kInitialCursorValue;
        int64_t counter = _retry_loop;

        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);

        while((available_sequence = min_sequence()) < sequence) {
            if(alerted.load()) {
                return kAlertedSignal;
            }
            counter = ApplyWaitMethod(counter);
        }
        return available_sequence;
    }

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted,
                    const std::chrono::microseconds& timeout) override {
        int64_t available_sequence = kInitialCursorValue;
        int64_t counter = _retry_loop;

        const auto start = std::chrono::system_clock::now();
        const auto stop = start + timeout;
        const auto min_sequence = buildMinSequenceFunction(cursor,dependents);

        while((available_sequence = min_sequence()) < sequence) {
            if(alerted.load()) {
                return kAlertedSignal;
            }
            counter = ApplyWaitMethod(counter);
            if(stop <= std::chrono::system_clock::now()) {
                return kTimeoutSignal;
            } 
        }
        return available_sequence;
    }

    virtual void SignalAllWhenBlocking() override {}

private:
    inline int64_t ApplyWaitMethod(int64_t counter) {
        if(counter > (_retry_loop / 2)) {
            --counter;
        }
        else if(counter > 0) {
            --counter;
            std::this_thread::yield();
        }
        else {
            std::this_thread::sleep_for(std::chrono::microseconds(_duration_value));
        }
        return counter;
    }

    int64_t _retry_loop;
    int64_t _duration_value;
};

class BlockingStrategy : public WaitStrategy
{
    DISALLOW_COPY_MOVE_AND_ASSIGN(BlockingStrategy);
    using Lock = std::unique_lock<std::recursive_mutex>;
    using Waiter = std::function<bool(Lock&)>;
public:
    explicit BlockingStrategy() {}

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted) override {
        return WaitFor(sequence,cursor,dependents,alerted,[this](Lock& lock){
            _consumer_notify_condition.wait(lock);
            return false;
        });
    }

    virtual int64_t WaitFor(const int64_t& sequence,
                    const Sequence& cursor,
                    const std::vector<Sequence*>& dependents,
                    const std::atomic<bool>& alerted,
                    const std::chrono::microseconds& timeout) override {
        return WaitFor(sequence,cursor,dependents,alerted,
        [this,timeout](Lock& lock) {
            return std::cv_status::timeout == 
                _consumer_notify_condition.wait_for(lock,std::chrono::microseconds(timeout));
        });
    }

    virtual void SignalAllWhenBlocking() override {
        Lock ulock(_mutex);
        _consumer_notify_condition.notify_all();
    }

private:
    inline int64_t WaitFor(const int64_t& sequence,
                           const Sequence& cursor,
                           const std::vector<Sequence*>& dependents,
                           const std::atomic<bool>& alerted,
                           const Waiter& locker) {
        int64_t available_sequence = kInitialCursorValue;

        // BlockingStrategy is a special case where the unblock signal
        // comes from Sequencer(cursor)
        // This is why we need to wait on the cursor first, then dependents
        if((available_sequence = cursor.GetSequence()) < sequence) {
            Lock ulock(_mutex);
            // cursor: Pointing to the next writable position in the ringbuffer
            while((available_sequence = cursor.GetSequence()) < sequence) {
                if(alerted.load()) {
                    return kAlertedSignal;
                }
                // wait(): Release the lock and re get the lock when wait()
                // function to be executed 
                // locker indicate if a timeout occured
                if(locker(ulock)) {
                    return kTimeoutSignal;
                }
            }
        }

        // Now wait on dependents
        if(dependents.size()) {
            while((available_sequence = GetMinimumSequence(dependents)) < sequence) {
                if(alerted.load()) {
                    return kAlertedSignal;
                }
            }
        }
        return available_sequence;
    }

    std::recursive_mutex _mutex;
    std::condition_variable_any _consumer_notify_condition;
};

static inline WaitStrategy* CreateWaitStrategy(WaitStrategyOption option) {
    WaitStrategy* strategy = nullptr;
    switch (option) {
    case kBlockingStrategy:
        strategy = new BlockingStrategy();
        break;
    case kSleepingStrategy:
        strategy = new SleepingStrategy();
        break;
    case kYieldingStrategy:
        strategy = new YieldingStrategy();
        break;
    case kBusySpinStrategy:
        strategy = new BusySpinStrategy();
        break;
    default:
        break;
    }
    return strategy;
}

static inline std::function<int64_t()> buildMinSequenceFunction(
    const Sequence& cursor,const std::vector<Sequence*>& dependents) {
    if(dependents.empty()) {
        return [&cursor]() -> int64_t {
            return cursor.GetSequence();
        };
    }
    else {
        return [&dependents]() -> int64_t {
            return GetMinimumSequence(dependents);
        };
    }
}

} // end namespace disruptor

#endif