#pragma once
#include <memory>
#include <iostream>

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
#define READ (EPOLLIN|EPOLLRDHUP)
#define WRITE (EPOLLOUT)
#define ALL (EPOLLIN|EPOLLOUT|EPOLLRDHUP)

class TcpBase {
public:
    TcpBase(int port, int evenType)
            :sock_(port),
             evenType_(evenType) {
                sock_.Init();
             }

    //Fixme tmp will never used! just nullptr!
    TcpBase(int fd, int *tmp, int evenType = READ)
            :sock_(0),
             evenType_(evenType) {
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