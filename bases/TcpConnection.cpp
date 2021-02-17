#include <string.h>
#include "TcpConnection.h"
TcpConnection::TcpConnection(int fd, int event, EventLoop *loop,
                            ReadCompleteCallBk read,
                            WriteCompleteCallBk write):fd_(fd), event_(event),
                            loop_(loop), c_(fd_, event_, bind(&TcpConnection::HandleInput, this, placeholders::_1),
                            bind(&TcpConnection::HandleOutput, this, placeholders::_1), loop_),
                            readHandler_(read), writeHandler_(write) {
    loop_->AddChannel(c_);
}

TcpConnection::~TcpConnection() {
}

int TcpConnection::HandleInput(void *data) {
    int ret = inputBuffer_.ReadFromFd(fd_);
    if(0 == ret) {
        loop_->DelChannel(c_);
        //TODO 调用tcpserver清除此conn
    }

    if(-1 == ret && errno == EAGAIN) {
        return -1;
    }

    LOG_DEBUG<<"prepare to do readHanler"<<endl;
    if(nullptr == this) {
        LOG_DEBUG<<"this is null"<<endl;
    }
    if(readHandler_) readHandler_(shared_from_this(), &inputBuffer_);
    return ret;
}

int TcpConnection::HandleOutput(void *data) {
    outputBuffer_.SendToFd(fd_);
    if(writeHandler_) writeHandler_(shared_from_this(), &outputBuffer_);
    return 0;
}

void TcpConnection::SendData(const char *data, int len) {
    outputBuffer_.AppenData(data, len);
}