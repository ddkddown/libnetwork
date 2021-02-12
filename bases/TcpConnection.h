#pragma once
#include "EventLoop.h"

//TODO 暴露出定制接口，例如clientRead, clientWrite, CloseHandler

class TcpConnection {
public:
    TcpConnection(int fd, int event, EventLoop *loop,
                EventReadCallback read,
                EventWriteCallback write);
    virtual ~TcpConnection();

private:
    int fd_;
    int event_;
    EventReadCallback readHandler_;
    EventWriteCallback writeHandler_;
    EventLoop *loop_;
    Channel c_;
};