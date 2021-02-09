#include <string.h>
#include "TcpConnection.h"
TcpConnection::TcpConnection(int fd, int event, EventLoop *loop):fd_(fd), event_(event),
                    loop_(loop), c_(fd_, event_, bind(&TcpConnection::HandleRead, *this),
                    bind(&TcpConnection::HandleWrite, *this), loop_) {
    loop_->AddChannel(c_);
}

TcpConnection::~TcpConnection(){}

int TcpConnection::Read(char *dst, int size) {
    return buff_.Read(dst, size);
}

int TcpConnection::Write(char *src, int size) {
    return buff_.Write(src, size);
}

int TcpConnection::HandleRead(void *data) {
    return buff_.ReaDataFromFd(fd_);
}

int TcpConnection::HandleWrite(void *data) {
    return buff_.WriteDataToFd(fd_);
}