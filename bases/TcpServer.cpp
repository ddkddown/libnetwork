#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "Logger.h"

TcpServer::IgnoreSig TcpServer::initSig_;

TcpServer::TcpServer(int port, int poolSize):
                    accpt_(port), pool_(poolSize),
                    readHandler_(nullptr), writeHandler_(nullptr) {
    LOG_DEBUG<<"accpt_ fd"<<accpt_.GetFd();
    Channel c(accpt_.GetFd(), EPOLLIN, 
            bind(&TcpServer::HandleAcceptor,
            this,std::placeholders::_1),
            nullptr, &pool_.GetMainLoop());
    pool_.GetMainLoop().AddAcceptor(c);
}

TcpServer::~TcpServer() {}

void TcpServer::Start() {
    pool_.Run();
}

int TcpServer::HandleAcceptor(void *data) {
    struct sockaddr_in clienAddr;
    socklen_t clienLen = sizeof(clienAddr);
    int clienFd = accept(accpt_.GetFd(), (struct sockaddr*)&clienAddr, &clienLen);
    if(-1 == clienFd) {
        LOG_ERR<<"accept client failed!";
        return -1;
    }

    EventLoop &loop = pool_.GetLoop();
    TcpConnectionPtr conn(new TcpConnection(clienFd, EPOLLIN|EPOLLET,
                        &loop, readHandler_, writeHandler_));
    conns_.push_back(conn);
    return clienFd;
}

int TcpServer::GetAcceptFd() {
    return accpt_.GetFd();
}

void TcpServer::SetReader(ReadCompleteCallBk read) {
    readHandler_ = read;
}
void TcpServer::SetWriter(WriteCompleteCallBk write) {
    writeHandler_ = write;
}