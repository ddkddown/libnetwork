#include "EchoServer.h"

int EchoServer::ReadHandler(int fd, void *data) {
    char buff[1024] = {0};
    recv(fd, buff, 1024, 0);
    send(fd, buff, 1024, 0);
    return 0;
}

int EchoServer::WriteHandler(int fd, void *data) {
    return 0;
}

EchoServer::EchoServer(int port, int poolSize):
                        TcpServer(port, poolSize) {
    TcpServer::SetWriter(bind(&EchoServer::ReadHandler, this,
                        std::placeholders::_1, std::placeholders::_2));
    TcpServer::SetWriter(bind(&EchoServer::WriteHandler, this,
                        std::placeholders::_1, std::placeholders::_2));
}

EchoServer::~EchoServer() {
    
}

int main() {
    EchoServer echo(9677, 10);
    echo.Start();
}