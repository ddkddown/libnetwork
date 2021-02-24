#include "EchoServer.h"
#include "Logger.h"


//TODO 修改handle read
/*
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

        //TODO EWOULDBLOCK and EINTR
        if(-1 == ret && errno == EAGAIN) {
            continue;
        }

        LOG_DEBUG<<"echo read: "<<buff<<endl;
        send(fd, buff, 1024, 0);
        break;

    } while (1);

    return 0;
}
*/
void EchoServer::MessageCall(const TcpConnectionPtr &conn, Buffer *buff) {
    LOG_DEBUG<<"read complete call"<<endl;
    char tmp[1024] = {0};
    int len = buff->GetData(tmp, 1024);
    send(conn->GetFd(), tmp, len, 0);
    return;
}

void EchoServer::WriteCompleteCall(const TcpConnectionPtr& conn) {
    return;
}

void EchoServer::ConnectionCall(const TcpConnectionPtr &conn) {
    return;
}

EchoServer::EchoServer(int port, int poolSize):
                        TcpServer(port, poolSize, bind(&EchoServer::ConnectionCall, this, placeholders::_1),
                                bind(&EchoServer::MessageCall, this, placeholders::_1, placeholders::_2),
                                bind(&EchoServer::WriteCompleteCall, this, placeholders::_1)) {
}

EchoServer::~EchoServer() {
    
}

int main() {
    Logger::InitLog("./echo.log");
    EchoServer echo(9677, 1);
    echo.Start();
}
