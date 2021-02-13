#include "EchoServer.h"
#include "Logger.h"

int EchoServer::ReadHandler(int fd, void *data) {
    char buff[1024] = {0};
    int ret = 0;
    do
    {
        ret = recv(fd, buff, 1024, 0);
        if(0 == ret) {
            close(fd);
            break;
        }

        if(-1 == ret && errno == EAGAIN) {
            continue;
        }

        LOG_DEBUG<<"echo read: "<<buff<<endl;
        send(fd, buff, 1024, 0);
        break;
        
    } while (1);

    return 0;
}

int EchoServer::WriteHandler(int fd, void *data) {
    return 0;
}

EchoServer::EchoServer(int port, int poolSize):
                        TcpServer(port, poolSize) {
    TcpServer::SetReader(bind(&EchoServer::ReadHandler, this,
                        std::placeholders::_1, std::placeholders::_2));
    TcpServer::SetWriter(bind(&EchoServer::WriteHandler, this,
                        std::placeholders::_1, std::placeholders::_2));
}

EchoServer::~EchoServer() {
    
}

int main() {
    Logger::InitLog("./echo.log");
    EchoServer echo(9677, 2);
    echo.Start();
    sleep(10000);
}