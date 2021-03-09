#pragma once
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <unistd.h>
#include "Channel.h"
#include "Poller.h"
#include "TimeRounder.h"

class EventLoop : boost::noncopyable {
public:
    using Functor = function<void()>;

    EventLoop();
    ~EventLoop();
    void Loop();
    void RunInLoop(const Functor &cb);
    void QueueInLoop(const Functor &cb);
    void UpdateChannel(Channel *c);
    void RemoveChannel(Channel *c);
    inline bool IsRunning() {
        return looping_;
    }
    void AddTimer(TimeCallback cb, int interval, bool repeat);
    void Quit();
    void SetPoller(const shared_ptr<Poller> &poller) {
        poller_ = poller;
    }
private:
    using ChannList = vector<Channel*>;
    void DoPendingFunctors();
    void HandleWakeUp();
    void wakeup();

    class Wakeup {
    public:
        Wakeup() {
            assert(0 == pipe(pipeFd_));
        }

        int GetReader() {
            return pipeFd_[0];
        }

        int GetWriter() {
            return pipeFd_[1];
        }

        ~Wakeup() {
            close(pipeFd_[0]);
            close(pipeFd_[1]);
        }
    private:
        int pipeFd_[2];
    };
private:
    bool looping_;
    bool quit_;
    bool eventHandling_;
    bool callingPendingFunctors_;
    Wakeup wake_;
    shared_ptr<Channel> wakeupChannel_;
    ChannList activeChannles_;
    shared_ptr<Poller> poller_;
    mutex m_;//TODO shared?
    vector<Functor> pendingFunctors_;
    TimeRounder rounder_;
};