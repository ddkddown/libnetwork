#pragma once
#include <functional>
#include "EventLoop.h"
#include "Buffer.h"

class TcpConnection;

using ReadCompleteCallBk = function<void(TcpConnection&, Buffer&)>;
using WriteCompleteCallBk = function<void(TcpConnection&, Buffer&)>;

class TcpConnection {
public:
    TcpConnection(int fd, int event, EventLoop *loop,
                ReadCompleteCallBk read = nullptr,
                WriteCompleteCallBk write = nullptr);
    virtual ~TcpConnection();

    int HandleInput(void *data);

    int HandleOutput(void *data);

    void SendData(const char *data, int len);

    inline int GetFd() {
        return fd_;
    }

private:
    int fd_;
    int event_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    ReadCompleteCallBk readHandler_;
    WriteCompleteCallBk writeHandler_;
    EventLoop *loop_;
    Channel c_;
};