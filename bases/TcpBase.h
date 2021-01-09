#pragma once
#include <memory>

extern "C" {
#include <sys/epoll.h>
#include <unistd.h>
}

#include "Socket.h"
class TcpBase;

using SharedBase = std::shared_ptr<TcpBase>;
using CSharedBaseRef = const std::shared_ptr<TcpBase>&;
#define READ (EPOLLIN|EPOLLET)
#define WRITE (EPOLLOUT|EPOLLET)
#define ALL (EPOLLIN|EPOLLOUT|EPOLLET)

class TcpBase {
public:
    TcpBase(int port, int evenType = ALL)
            :sock_(port),
             evenType_(evenType){}

    ~TcpBase() {
        close(sock_.GetSockFd());
    }

    int GetFd() {
        return sock_.GetSockFd();
    }

    int GetEvent() {
        return evenType_;
    }

    virtual void ReadHandle() = 0;
    virtual void WriteHandle() = 0;
protected:
    network::SocketWrapper sock_;
    int evenType_;
};