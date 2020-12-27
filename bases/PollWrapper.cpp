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
    HandleEvent();
}