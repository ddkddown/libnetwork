#pragma once
#include <iostream>
#include <map>

extern "C" {
#include <sys/epoll.h>
#include <unistd.h>
}
#include "Nocopyable.h"
#include "Logger.h"
#include "FdWrapper.h"

namespace network {
    using READ = EPOLLIN;
    using WRITE = EPOLLOUT;
};

class PollWrapper : public network::Nocopy {
private:
    #define DEFAULT_SIZE 1024
public:
    PollWrapper(int eventSize = DEFAULT_SIZE)
                :eventSize_(eventSize) {
        epollFd_ = epoll_create(eventSize_);
        LOG_INFO<<"epoll created.";
    }

    ~PollWrapper() {
        close(epollfd);
    }
    
    void addEvent(FdWrapper &fd);

    void deleteEvent(FdWrapper &fd);

    void Run();
private:
    void HandleEvent();
private:
    struct epoll_event event_;
    int epollFd_;
    int eventSize_;
    std::map<int, FdWrapper> fdMap_;
};