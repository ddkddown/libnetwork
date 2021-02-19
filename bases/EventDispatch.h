#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Channel.h"
using namespace std;

class EventDispatch {
using ChanneList = vector<Channel*>;
using EventList = vector<struct epoll_event>;
using ChannelMap = map<int, Channel*>;

public:
    EventDispatch(void *loop);
    ~EventDispatch();
    void DeleteChannel(Channel *c);
    void UpdateChannel(Channel *c);
    void Dispatch(ChanneList *activeChannels);
private:
    void Update(int op, Channel *channel);
    enum {
        InitEventSize = 16
    };
    int efd_;
    EventList events_;
    ChannelMap channels_;
    void *loop_;
};