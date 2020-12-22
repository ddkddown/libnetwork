#pragma once
#include <iostream>

extern "C" {
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}

namespace network {
class IgnoreSig {
public:
    IgnoreSig() {
        ::signal(SIGPIPE, SIG_IGN);
    }
};

IgnoreSig initSig;

class SocketWrapper {
//TODO RAII tcpServer
private:
struct sockaddr_in myAddr_;
int sockFd;

};
};