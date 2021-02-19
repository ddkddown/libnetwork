#include <fcntl.h>
#include <errno.h>
#include <sys/eventfd.h>
#include "EventLoop.h"
#include "Logger.h"

static int CreateWakeFd() {
    int wakeFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(wakeFd < 0) {
        LOG_ERR<<"create eventFd failed!";
        abort();
    }

    return wakeFd;
}

EventLoop::EventLoop()
        : looping_(false),
          quit_(false),
          eventHandling_(false),
          callingPendingFunctors_(false),
          dispatcher_(this),
          wakeFd_(CreateWakeFd()),
          wakeupChannel_(new Channel(this, wakeFd_)) {
    
    wakeupChannel_->SetReadCallbk(bind(&EventLoop::HandleWakeUp, this));
    wakeupChannel_->EnableRead();
}

EventLoop::~EventLoop() {
    close(wakeFd_);
}

void EventLoop::Loop() {
    assert(!looping_);
    looping_ = true;
    while(!quit_) {
        activeChannles_.clear();
        dispatcher_.Dispatch(&activeChannles_);
        eventHandling_ = true;
        for(auto i : activeChannles_) {
            i->HandleEvent();
        }
        eventHandling_ = false;
        DoPendingFunctors();
    }
    looping_ = false;
}


int EventLoop::Quit() {
    quit_ = true;
    wakeup();
}

void EventLoop::wakeup() {
    char one = 1;
    write(wakeFd_, &one, sizeof one);
}

void EventLoop::RunInLoop(const Functor &cb) {
    cb();
}

void EventLoop::QueueInLoop(const Functor &cb) {
    {
        lock_guard<mutex> lk(m_);
        pendingFunctors_.push_back(cb);
    }

    wakeup();
}

void EventLoop::UpdateChannel(Channel *c) {
    dispatcher_.UpdateChannel(c);
}

void EventLoop::RemoveChannel(Channel *c) {
    dispatcher_.DeleteChannel(c);
}

void EventLoop::HandleWakeUp() {
    char one = 1;
    read(wakeFd_, &one, sizeof one);
}

void EventLoop::DoPendingFunctors() {
    vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        //func可能执行时间长, 减小锁粒度
        lock_guard<mutex> lk(m_);
        functors.swap(pendingFunctors_);
    }

    for(auto i : functors) {
        i();
    }

    callingPendingFunctors_ = false;
}