#pragma once
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <unistd.h>
#include "Channel.h"
#include "EventDispatch.h"

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
    int Quit();
private:
    using ChannList = vector<Channel*>;
    void DoPendingFunctors();
    void HandleWakeUp();
    void wakeup();

private:
    bool looping_;
    bool quit_;
    bool eventHandling_;
    bool callingPendingFunctors_;
    int wakeFd_;
    shared_ptr<Channel> wakeupChannel_;
    ChannList activeChannles_;
    EventDispatch dispatcher_;
    mutex m_;//TODO shared?
    vector<Functor> pendingFunctors_;
};