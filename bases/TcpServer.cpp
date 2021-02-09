#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "Logger.h"

TcpServer::IgnoreSig TcpServer::initSig_;

TcpServer::TcpServer(int port, int poolSize):
                    accpt_(port), pool_(poolSize) {}

TcpServer::~TcpServer() {}

void TcpServer::Start() {
    Channel c(accpt_.GetFd(), EPOLLIN, 
            bind(&TcpServer::HandleAccptor, *this), nullptr, &pool_.GetMainLoop());
    pool_.GetMainLoop().AddChannel(c);
}

int TcpServer::HandleAccptor(void *data = nullptr) {
    struct sockaddr_in clienAddr;
    socklen_t clienLen = sizeof(clienAddr);
    int clienFd = accept(accpt_.GetFd(), (struct sockaddr*)&clienAddr, &clienLen);
    if(-1 == clienFd) {
        LOG_ERR<<"accept client failed!";
        return -1;
    }

    EventLoop &loop = pool_.GetLoop();
    TcpConnection conn(clienFd, EPOLLIN, &loop);

    return clienFd;
}