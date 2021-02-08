#pragma once
#include <iostream>
#include "Channel.h"
using namespace std;

class EventDispatch {
public:
    EventDispatch(void *loop);
    ~EventDispatch();
    int AddChannel(Channel &c);
    int DeleteChannel(Channel &c);
    int UpdateChannel(Channel &c);
    void Dispatch();
    void Clear();
private:
    enum {
        MAX_EVENT = 1024
    };

    int eventCount_;
    int fds_;
    int efd_;
    void *loop_;
    struct epoll_event *events_;
};