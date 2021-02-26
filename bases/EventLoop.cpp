#include <fcntl.h>
#include <errno.h>
#include <sys/eventfd.h>
#include "EventLoop.h"
#include "Logger.h"


/*
static int CreateWakeFd() {
    int wakeFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(wakeFd < 0) {
        LOG_ERR<<"create eventFd failed!";
        abort();
    }

    return wakeFd;
}
*/

EventLoop::EventLoop()
        : looping_(false),
          quit_(false),
          eventHandling_(false),
          callingPendingFunctors_(false),
          dispatcher_(this),
          wake_(),
          wakeupChannel_(new Channel(this, wake_.GetReader())) {
    wakeupChannel_->SetReadCallbk(bind(&EventLoop::HandleWakeUp, this));
    wakeupChannel_->EnableRead();
}

EventLoop::~EventLoop() {
}

void EventLoop::Loop() {
    assert(!looping_);
    looping_ = true;
    while(!quit_) {
        activeChannles_.clear();
        LOG_DEBUG<<"dispatching"<<endl;
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


void EventLoop::Quit() {
    if(quit_) {
        return;
    }
    quit_ = true;
    wakeup();
}

void EventLoop::wakeup() {
    char one = 1;
    write(wake_.GetWriter(), &one, sizeof one);
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
    LOG_DEBUG<<"handle wake up"<<endl;

    char one = 1;
    ssize_t size = read(wake_.GetReader(), &one, sizeof one);
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