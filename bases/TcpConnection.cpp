#include <string.h>
#include "TcpConnection.h"
TcpConnection::TcpConnection(int fd, int event, EventLoop *loop,
                            function<EventReadCallback> read,
                            function<EventWriteCallback> write):fd_(fd), event_(event),
                            loop_(loop), c_(fd_, event_, readHandler_, writeHandler_, loop_),
                            readHandler_(read), writeHandler_(write) {
    loop_->AddChannel(c_);
}

TcpConnection::~TcpConnection(){}

int TcpConnection::Read(char *dst, int size) {
    return buff_.Read(dst, size);
}

int TcpConnection::Write(char *src, int size) {
    return buff_.Write(src, size);
}