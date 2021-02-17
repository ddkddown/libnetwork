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
void EchoServer::ReadCompleteCall(const TcpConnectionPtr &conn, Buffer *buff) {
    LOG_DEBUG<<"read complete call start"<<endl;
    char tmp[1024] = {0};
    int len = buff->GetData(tmp, 1024);
    LOG_DEBUG<<"echo read: "<<tmp <<"len is "<<len<<endl;
    send(conn->GetFd(), tmp, len, 0);
    return;
}

void EchoServer::WriteCompleteCall(const TcpConnectionPtr& conn, Buffer *buff) {
    return;
}

EchoServer::EchoServer(int port, int poolSize):
                        TcpServer(port, poolSize) {
    TcpServer::SetReader(bind(&EchoServer::ReadCompleteCall, this,
                        placeholders::_1, placeholders::_2));
    TcpServer::SetWriter(bind(&EchoServer::WriteCompleteCall, this,
                        placeholders::_1, placeholders::_2));
}

EchoServer::~EchoServer() {
    
}

int main() {
    Logger::InitLog("./echo.log");
    EchoServer echo(9677, 2);
    echo.Start();
}