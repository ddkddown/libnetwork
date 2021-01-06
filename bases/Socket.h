#pragma once
#include <iostream>

extern "C" {
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
}

namespace network {
class IgnoreSig {
public:
    IgnoreSig() {
        ::signal(SIGPIPE, SIG_IGN);
    }
};

class SocketWrapper {
public:
    SocketWrapper(int port) {
        sockFd_ = socket(AF_INET, SOCK_STREAM, 0);
        myAddr_.sin_family = AF_INET;
        myAddr_.sin_port = htons(port);
        myAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    void Bind() {
        assert(0 == bind(sockFd_, (struct sockaddr*)&myAddr_,
                                         sizeof myAddr_));
    }

    void Listen(int size) {
        listen(sockFd_, size);
    }

    int Accept() {
        socklen_t peerLen = sizeof peerAddr_;
        return accept(sockFd_, (struct sockaddr*)&peerAddr_, &peerLen);
    }

    int GetSockFd() {
        return sockFd_;
    }

private:
    struct sockaddr_in myAddr_;
    struct sockaddr_in peerAddr_;
    int sockFd_;
};
};