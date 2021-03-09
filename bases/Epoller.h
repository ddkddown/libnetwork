#pragma once
#include <iostream>
#include <map>
#include "Poller.h"
using namespace std;

class Epoller : public Poller {
using EventList = vector<struct epoll_event>;
using ChannelMap = map<int, Channel*>;
public:
    Epoller(void *loop);
    ~Epoller();
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
};