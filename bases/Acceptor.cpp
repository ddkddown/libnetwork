#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "Acceptor.h"
#include "Logger.h"

Acceptor::Acceptor(EventLoop *loop, int port):loop_(loop), port_(port),
                    cliLen_(sizeof(cliaddr_)) {
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
        int on = 1;
        if(-1 == setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, static_cast<socklen_t>(sizeof on))) {
            LOG_WARN<<"set reuseaddr failed: "<<strerror(errno)<<endl;
        }
        if(bind(fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            LOG_ERR<<"bind socket failed!"<<endl;
            break;
        }

    } while (0);

    acceptChannel_ = make_shared<Channel>(loop, fd_);
    acceptChannel_->SetReadCallbk(bind(&Acceptor::HandleRead, this));
}

Acceptor::~Acceptor() {
    shutdown(fd_, SHUT_RDWR);
    close(fd_);
    close(extraFd_);
}

void Acceptor::OpenDevNull() {
    extraFd_ = open("/dev/null", 0);
    assert(-1 != extraFd_);
}

void Acceptor::Listen() {
    listen(fd_, LISTENQ);
    acceptChannel_->EnableRead();
}

void Acceptor::HandleRead() {
    //TODO 文件描述符耗完时的处理
    struct sockaddr_in cliaddr;
    int clientFd = accept(fd_, (sockaddr*)&cliaddr_, &cliLen_);
    if(0 <= clientFd && newConnCallBk_) {
        newConnCallBk_(clientFd);
    }
    else if (-1 == clientFd && errno == EMFILE) {
        close(extraFd_);
        clientFd = accept(fd_, (sockaddr*)&cliaddr_, &cliLen_);
        assert(-1 != clientFd);
        close(clientFd);
        OpenDevNull();
        LOG_WARN<<"too many open files!"<<endl;
    }
    else {
        LOG_WARN<<"accept error: "<<clientFd<<strerror(errno)<<endl;
    }
}
