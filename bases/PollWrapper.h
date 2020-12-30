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
using namespace network;

namespace {
    #define READ (EPOLLIN|EPOLLET)
    #define WRITE (EPOLLOUT|EPOLLET)
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
        close(epollFd_);
    }
    
    void addEvent(FdWrapper &fd);

    void deleteEvent(FdWrapper &fd);

    void Run();
private:
    void HandleEvent(struct epoll_event *events, int size);
private:
    int epollFd_;
    int eventSize_;
    std::map<int, FdWrapper> fdMap_;
    struct epoll_event event_;
};