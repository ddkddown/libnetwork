#include "EventDispatch.h"
#include "EventLoop.h"
#include "Logger.h"

EventDispatch::EventDispatch(void* loop):
                eventCount_(0),fds_(0),
                efd_(0),events_(nullptr),
                loop_(loop) {
    efd_ = epoll_create(0);
    if(-1 == efd_) {
        LOG_ERR<<"create epoll failed!";
    }

    try {
        events_ = new epoll_event[MAX_EVENT];
    } catch(...) {
        LOG_ERR<<"alloc epoll event failed!";
    }
}

EventDispatch::~EventDispatch() {
    Clear();
}

int EventDispatch::AddChannel(Channel &c) {
    struct epoll_event event;
    event.data.fd = c.GetFd();
    event.events = c.GetEvent();
    
    if(-1 == epoll_ctl(efd_, EPOLL_CTL_ADD, c.GetFd(), &event)) {
        LOG_ERR<<"error while epoll add fd";
        return -1;
    }

    return 0;
}

int EventDispatch::DeleteChannel(Channel &c) {
    struct epoll_event event;
    event.data.fd = c.GetFd();
    event.events = 0;
    
    if(-1 == epoll_ctl(efd_, EPOLL_CTL_DEL, c.GetFd(), &event)) {
        LOG_ERR<<"error while epoll delete fd";
        return -1;
    }

    return 0;
}

int EventDispatch::UpdateChannel(Channel &c) {
    struct epoll_event event;
    event.data.fd = c.GetFd();
    event.events = c.GetEvent();
    
    if(-1 == epoll_ctl(efd_, EPOLL_CTL_MOD, c.GetFd(), &event)) {
        LOG_ERR<<"error while epoll update fd";
        return -1;
    }

    return 0;
}

void EventDispatch::Dispatch() {
    auto num = epoll_wait(efd_, events_, MAX_EVENT, -1);
    EventLoop *loop = static_cast<EventLoop*>(loop_);
    for(int i = 0; i < num; ++i) {
        loop->EventActive(events_[i].data.fd, events_[i].events);
    }

    return;
}

void EventDispatch::Clear() {
    if(nullptr != events_) {
        delete [] events_;
    }
    close(efd_);
}