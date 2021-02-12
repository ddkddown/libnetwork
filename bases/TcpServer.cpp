#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "Logger.h"

TcpServer::IgnoreSig TcpServer::initSig_;

TcpServer::TcpServer(int port, int poolSize):
                    accpt_(port), pool_(poolSize),
                    readHandler_(nullptr), writeHandler_(nullptr) {}

TcpServer::~TcpServer() {}

void TcpServer::Start() {
    Channel c(accpt_.GetFd(), EPOLLIN, 
            bind(&TcpServer::HandleAcceptor, this,
            std::placeholders::_1, std::placeholders::_2),
            nullptr, &pool_.GetMainLoop());
    pool_.GetMainLoop().AddChannel(c);
}

int TcpServer::HandleAcceptor(int fd, void *data) {
    struct sockaddr_in clienAddr;
    socklen_t clienLen = sizeof(clienAddr);
    int clienFd = accept(accpt_.GetFd(), (struct sockaddr*)&clienAddr, &clienLen);
    if(-1 == clienFd) {
        LOG_ERR<<"accept client failed!";
        return -1;
    }

    EventLoop &loop = pool_.GetLoop();
    TcpConnection conn(clienFd, EPOLLIN, &loop, readHandler_, writeHandler_);

    return clienFd;
}

int TcpServer::GetAcceptFd() {
    return accpt_.GetFd();
}

void TcpServer::SetReader(EventReadCallback read) {
    readHandler_ = read;
}
void TcpServer::SetWriter(EventWriteCallback write) {
    writeHandler_ = write;
}