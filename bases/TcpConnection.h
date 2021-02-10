#pragma once
#include "EventLoop.h"
#include "Buffer.h"

//TODO 暴露出定制接口，例如clientRead, clientWrite, CloseHandler

class TcpConnection {
public:
    TcpConnection(int fd, int event, EventLoop *loop,
                function<EventReadCallback> read,
                function<EventWriteCallback> write);
    virtual ~TcpConnection();
    int Read(char *dst, int size);
    int Write(char *src, int size);
    
private:
    int fd_;
    int event_;
    function<EventReadCallback> readHandler_;
    function<EventWriteCallback> writeHandler_;
    EventLoop *loop_;
    Channel c_;
    Buffer buff_;
};