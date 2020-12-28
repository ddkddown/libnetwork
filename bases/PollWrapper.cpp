#include "PollWrapper.h"

void PollWrapper::addEvent(FdWrapper &fd) {
    event_.events = fd.GetEventType();
    event_.data.fd = fd.GetRawFd();
    epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd.GetRawFd(), &event_);
    fdMap_[fd.GetRawFd()] = fd;
}

void PollWrapper::deleteEvent(FdWrapper &fd) {
    event_.events = fd.GetEventType();
    event_.data.fd = fd.GetRawFd();
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd.GetRawFd(), &event_);
    fdMap_.erase(fdMap_.find(fd.GetRawFd()));

    
}

void PollWrapper::Run() {
    struct epoll_event events[eventSize_];
    auto ret = epoll_wait(epollFd_, events, eventSize_, -1);
    HandleEvent(&events[0], ret);
}

void PollWrapper::HandleEvent(struct epoll_event *events, 
                                                int size) {
    for(int i = 0; i < size; ++i) {
        if(EPOLLIN & events[i].events) {
            auto fdWrapper = fdMap_[events[i].data.fd];
            fdWrapper.GetReader()();
            continue;
        }

        if(EPOLLOUT & events[i].events) {
            auto fdWrapper = fdMap_[events[i].data.fd];
            fdWrapper.GetWriter()();
            continue;
        }

        LOG_WARN<<"unknown events:"<<events[i].events;
    }
}