#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "Acceptor.h"
#include "Logger.h"

Acceptor::Acceptor(int port):port_(port) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd_, F_SETFL, O_NONBLOCK);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    do
    {
        //no timewait
        char on = 1;
        setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if(bind(fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            LOG_ERR<<"bind socket failed!";
            break;
        }

        if(listen(fd_, LISTENQ) < 0) {
            LOG_ERR<<"listen fd_ "<<fd_<<" failed";
            break;
        }
    } while (0);
}

Acceptor::~Acceptor() {
    shutdown(fd_, SHUT_RDWR);
    close(fd_);
}

int Acceptor::GetFd() {
    return fd_;
}