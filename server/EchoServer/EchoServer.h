#pragma once
#include "TcpServer.h"
class EchoServer : public TcpServer{
public:
    EchoServer(int port, int poolSize);
    ~EchoServer();
    void MessageCall(const TcpConnectionPtr &conn, Buffer *buff);
    void WriteCompleteCall(const TcpConnectionPtr &conn);
    void ConnectionCall(const TcpConnectionPtr &conn);
};