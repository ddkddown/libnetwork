#pragma once
#include <iostream>
#include "Channel.h"
#include "EventLoop.h"
using namespace std;

class EventDispatch {
public:
    EventDispatch();
    ~EventDispatch();
    int AddChannel(Channel &c);
    int DeleteChannel(Channel &c);
    int UpdateChannel(Channel &c);
    int Dispatch(struct timeval &timeval);
    void Clear();
private:
    enum {
        MAX_EVENT = 1024
    };

    int eventCount_;
    int fds_;
    int efd_;
    struct epoll_event *events_;\
    void *data;
};