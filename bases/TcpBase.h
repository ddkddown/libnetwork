#pragma once
#include <memory>

extern "C" {
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
}

#include "Logger.h"
#include "Socket.h"
class TcpBase;

using SharedBase = std::shared_ptr<TcpBase>;
using CSharedBaseRef = const std::shared_ptr<TcpBase>&;
//#define READ (EPOLLIN|EPOLLRDHUP|EPOLLT)
#define READ (EPOLLIN|EPOLLRDHUP)
//#define WRITE (EPOLLOUT|EPOLLET)
#define WRITE (EPOLLOUT)
//#define ALL (EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET)
#define ALL (EPOLLIN|EPOLLOUT|EPOLLRDHUP)

class TcpBase {
public:
    TcpBase(int port, int evenType)
            :sock_(port),
             evenType_(evenType) {
                sock_.Init();
             }

    TcpBase(int fd)
            :sock_(0),
             evenType_(ALL) {
                sock_.SetSockFd(fd);
            }
    ~TcpBase() {
        close(sock_.GetSockFd());
    }

    int GetFd() {
        return sock_.GetSockFd();
    }

    int GetEvent() {
        return evenType_;
    }

    void CloseFd() {
        sock_.CloseFd();
    }

    virtual void ReadHandle() = 0;

    virtual void WriteHandle() = 0;
protected:
    network::SocketWrapper sock_;
    int evenType_;
};