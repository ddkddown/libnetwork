#include <string.h>
#include "TcpConnection.h"
TcpConnection::TcpConnection(int fd, int event, EventLoop *loop,
                            ReadCompleteCallBk read,
                            WriteCompleteCallBk write):fd_(fd), event_(event),
                            loop_(loop), c_(fd_, event_, bind(&TcpConnection::HandleInput, *this, placeholders::_1),
                            bind(&TcpConnection::HandleOutput, *this, placeholders::_1), loop_),
                            readHandler_(read), writeHandler_(write) {
    loop_->AddChannel(c_);
}

TcpConnection::~TcpConnection() {
}

int TcpConnection::HandleInput(void *data) {
    int ret = inputBuffer_.ReadFromFd(fd_);
    if(readHandler_) readHandler_(*this, inputBuffer_);
    return ret;
}

int TcpConnection::HandleOutput(void *data) {
    outputBuffer_.SendToFd(fd_);
    if(writeHandler_) writeHandler_(*this, outputBuffer_);
    return 0;
}

void TcpConnection::SendData(const char *data, int len) {
    outputBuffer_.AppenData(data, len);
}