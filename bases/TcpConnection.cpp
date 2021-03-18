#include <string.h>
#include "TcpConnection.h"
TcpConnection::TcpConnection(EventLoop *loop, int sockfd, Id id, TIMESTAMP now)
                :loop_(loop), fd_(sockfd), state_(CONNECTING), id_(id),
                channel_(new Channel(loop, sockfd)), now_(now) {
    channel_->SetReadCallbk(bind(&TcpConnection::HandleRead, this));
    channel_->SetWriteCallbk(bind(&TcpConnection::HandleWrite, this));
    channel_->SetCloseCallbk(bind(&TcpConnection::HandleClose, this));
    channel_->SetErrorCallbk(bind(&TcpConnection::HandleError, this));
}

TcpConnection::~TcpConnection() {
}

void TcpConnection::Send(Buffer *buff) {
    if(CONNECTED == state_) {
        loop_->RunInLoop(bind(&TcpConnection::SendInLoop, 
                this, buff->GetDataAString()));
    }
}

void TcpConnection::ShutDown() {
    if(CONNECTED == state_) {
        SetState(DISCONNECTING);
        loop_->RunInLoop(bind(&TcpConnection::ShutdownInLoop, this));
    }
}

void TcpConnection::SendInLoop(const string &data) {

}

void TcpConnection::ShutdownInLoop() {
    if(!channel_->IsWriting()) {
        shutdown(fd_, SHUT_WR);
    }
}

void TcpConnection::ConnectEstablished() {
    assert(CONNECTING == state_);
    SetState(CONNECTED);
    channel_->Tie(shared_from_this());
    channel_->EnableRead();

    connectionCallBk_(shared_from_this());
}

void TcpConnection::HandleRead() {
    int ret = inputBuffer_.ReadFromFd(fd_);
    if(0 == ret || -1 == ret) {
        HandleClose();
    }
    else if (0 < ret) {
        now_ = chrono::system_clock::now();
        messageCallBk_(shared_from_this(), &inputBuffer_);
    }
    else {
        LOG_WARN<<"ret: "<<ret<<"errno: "<<strerror(errno);
    }
}

void TcpConnection::HandleWrite() {
    outputBuffer_.SendToFd(fd_);
    if(writeCallBk_) writeCallBk_(shared_from_this());
}

void TcpConnection::ConnectDestroyed() {
    LOG_DEBUG<<"connect destroy"<<endl;
    //先关闭事件，再close掉fd
    if(CONNECTED == state_) {
        SetState(DISCONNECTED);
        channel_->DisableAll();
    }
    
    loop_->RemoveChannel(channel_.get());
    close(fd_);
}

void TcpConnection::HandleClose() {
    SetState(DISCONNECTED);
    channel_->DisableAll();
    closeCallBk_(shared_from_this());
}

void TcpConnection::HandleError() {
    LOG_WARN<<"get socket: "<<fd_<<" error"<<endl;
}
