#include <memory>
#include "EchoServer.h"
#include "Logger.h"

void EchoServer::MessageCall(const TcpConnectionPtr &conn, Buffer *buff) {
    LOG_DEBUG<<"read complete call"<<endl;
    char tmp[1024] = {0};
    int len = buff->GetData(tmp, 1024);
    LOG_DEBUG<<tmp<<endl;
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

std::shared_ptr<EchoServer> p;
void SigTermHandler(int s) {
    LOG_INFO<<"recv sigterm"<<endl;
    p->Quit();
}

int main() {
    signal(SIGTERM, SigTermHandler);
    Logger::InitLog("./echo.log");
    p = make_shared<EchoServer>(9677, 10);
    p->Start();
}
