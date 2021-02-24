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
            accpt_(new Acceptor(port, &pool_.GetMainLoop())), pool_(poolSize), connectionCallBk_(cn),
            messageCallBk_(ms), writeCompleteBk_(wr) {
    accpt_->SetNewConnCallBk(bind(&TcpServer::NewConn, this, placeholders::_1));
}

TcpServer::~TcpServer() {
    for(auto i : connections_) {
        auto conn = i.second;
        i.second.reset();
        conn->GetLoop()->RunInLoop(bind(&TcpConnection::ConnectDestroyed, conn));
        conn.reset();
    }
}

void TcpServer::Start() {
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
    conn->SetCloseCallBk(bind(&TcpServer::RemoveConnection, this, _1));
    //可以用ioloop::queueInLoop吗？，不能在mainloop里阻塞太久， 
    pool_.GetMainLoop.RunInLoop(boost::bind(&TcpConnection::ConnectEstablished, conn));
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& conn) {
    auto n = connections_.erase(conn->GetFd());
    assert(1 == n);
    EventLoop &loop = pool_.GetLoop();
    loop.QueueInLoop(bind(&TcpConnection::ConnectDestroyed, conn));
}