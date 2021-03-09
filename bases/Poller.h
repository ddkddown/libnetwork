#pragma once
#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include "Channel.h"

using namespace std;
using ChanneList = vector<Channel*>;

class Poller {
public:
    Poller(void *loop):loop_(loop) {}
    virtual ~Poller(){}
    virtual void DeleteChannel(Channel *c) = 0;
    virtual void UpdateChannel(Channel *c) = 0;
    virtual void Dispatch(ChanneList *activeChannels) = 0;

    void *loop_;
};