#pragma once
#include <iostream>
#include <map>

extern "C" {
#include <sys/epoll.h>
#include <unistd.h>
}
#include "Nocopyable.h"
#include "Logger.h"
#include "TcpBase.h"
using namespace network;

class PollWrapper : public network::Nocopy {
private:
    #define DEFAULT_SIZE 1024
public:
    PollWrapper(int eventSize = DEFAULT_SIZE)
                :eventSize_(eventSize) {
        epollFd_ = epoll_create(eventSize_);
    }

    ~PollWrapper() {
        close(epollFd_);
    }
    
    void addEvent(CSharedBaseRef fd);

    void deleteEvent(CSharedBaseRef fd);

    void Run();
private:
    void HandleEvent(struct epoll_event *events, int size);
private:
    int epollFd_;
    int eventSize_;
    std::map<int, SharedBase> fdMap_;
    struct epoll_event event_;
};