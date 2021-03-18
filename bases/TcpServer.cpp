#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "Logger.h"

static Id GenUUId() {
   uuid_t u;
   char buf[36];
   uuid_generate_random(u);
   uuid_unparse(u, buf);
   return buf;
}

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
            messageCallBk_(ms), writeCompleteBk_(wr), started_(false), heartBeat_(false), heartBeatNum_(0) {
    accpt_->SetNewConnCallBk(bind(&TcpServer::NewConn, this, placeholders::_1));

    //绑定默认心跳回调处理函数
    heartBeatCallBk_ = bind(&TcpServer::DefaultHeartBeatHandler, this, std::placeholders::_1);
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
        TcpConnectionPtr conn = i.second;
        i.second.reset();
        conn->GetLoop()->QueueInLoop(
            bind(&TcpConnection::ConnectDestroyed, conn)
        );
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
    auto id = GenUUId();
    TcpConnectionPtr conn(new TcpConnection(&ioLoop, sockFd, id, chrono::system_clock::now()));
    connections_[id] = conn;
    conn->SetConnectionCallBk(connectionCallBk_);
    conn->SetWriteCompleteCallBk(writeCompleteBk_);
    conn->SetMessageCallBk(messageCallBk_);
    conn->SetCloseCallBk(bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
    pool_.GetMainLoop().RunInLoop(bind(&TcpConnection::ConnectEstablished, conn));

    if(heartBeat_) {
        //开启心跳
        //添加心跳定时任务
        //ioLoop.AddTimer();
    }
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& conn) {
    pool_.GetMainLoop().QueueInLoop(bind(&TcpServer::RemoveConnectionInLoop, this, conn));
}

void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& conn) {
    LOG_DEBUG<<"remove fd"<<conn->GetId()<<"conn count:"<<conn.use_count()<<endl;
    auto n = connections_.erase(conn->GetId());
    assert(1 == n);
    EventLoop *ioLoop = conn->GetLoop();
    ioLoop->QueueInLoop(bind(&TcpConnection::ConnectDestroyed, conn));
}

void TcpServer::DefaultHeartBeatHandler(const TcpConnectionPtr& conn) {

    //TODO 若超过心跳响应次数，则关闭连接
    if(conn->GetDuraion().count() >= HEART_BEAT_TIME) {
        if(conn->AddHeartBeat() >= heartBeatNum_) {
            pool_.GetMainLoop().QueueInLoop(bind(&TcpServer::RemoveConnectionInLoop, this, conn));
            return;
        }
    }

    //TODO 向客户端发送心跳包
    //TODO 非阻塞
    char buff[10] = "ping";
    if(-1 == send(conn->GetFd(), buff, 10, 0)) {
        LOG_WARN<<"send heart beat failed!"<<endl;
    }
}