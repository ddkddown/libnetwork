#include "PollWrapper.h"

void PollWrapper::addEvent(CSharedBaseRef fd) {
    event_.events = fd->GetEvent();
    event_.data.fd = fd->GetFd();
    epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd->GetFd(), &event_);
    fdMap_[fd->GetFd()] = fd;
}

void PollWrapper::deleteEvent(CSharedBaseRef fd) {
    event_.events = fd->GetEvent();
    event_.data.fd = fd->GetFd();
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd->GetFd(), &event_);
    fdMap_.erase(fdMap_.find(fd->GetFd()));
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
            auto fd = fdMap_[events[i].data.fd];
            fd->ReadHandle();
            continue;
        }

        if(EPOLLOUT & events[i].events) {
            auto fd = fdMap_[events[i].data.fd];
            fd->WriteHandle();
            continue;
        }

        LOG_WARN<<"unknown events:"<<events[i].events;
    }

    this->Run();
}