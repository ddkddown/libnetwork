#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "Logger.h"

class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
  }
};

IgnoreSigPipe initObj;

TcpServer::TcpServer(int port, int poolSize, const ConnetionCallBk &cn,
            const MessageCallBk &ms, const WriteCompleteBk &wr):
            accpt_(new Acceptor(&pool_.GetMainLoop(), port)), pool_(poolSize), connectionCallBk_(cn),
            messageCallBk_(ms), writeCompleteBk_(wr), started_(false) {
    accpt_->SetNewConnCallBk(bind(&TcpServer::NewConn, this, placeholders::_1));
}

TcpServer::~TcpServer() {
    if(started_) {
        Quit();
    }
}

void TcpServer::Quit() {
    assert(true == started_);
    started_ = false;

    //先停止pool
    pool_.Quit();

    for(auto &i : connections_) {
        i.second->ConnectDestroyed();
    }
}
void TcpServer::Start() {
    assert(false == started_);
    started_ = true;
    accpt_->Listen();
    pool_.Run();
}

void TcpServer::NewConn(int sockFd) {
    EventLoop &ioLoop = pool_.GetLoop();
    TcpConnectionPtr conn(new TcpConnection(&ioLoop, sockFd));
    connections_[sockFd] = conn;
    conn->SetConnectionCallBk(connectionCallBk_);
    conn->SetWriteCompleteCallBk(writeCompleteBk_);
    conn->SetMessageCallBk(messageCallBk_);
    conn->SetCloseCallBk(bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
    pool_.GetMainLoop().RunInLoop(bind(&TcpConnection::ConnectEstablished, conn));
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& conn) {
    auto n = connections_.erase(conn->GetFd());
    assert(1 == n);
}