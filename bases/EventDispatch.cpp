#include "EventDispatch.h"
#include "Logger.h"

EventDispatch::EventDispatch():
                eventCount_(0),fds_(0),
                efd_(0),events_(nullptr) {
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
    if(nullptr != events_) {
        delete [] events_;
    }
}