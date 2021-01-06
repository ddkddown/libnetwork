#pragma once
#include "TcpServer.h"
#include "EchoClient.h"

class EchoServer : public TcpServer{
public:
    EchoServer(int port, int lSize)
            :TcpServer(port, lSize) {}
    void ReadHandle();
    void WriteHandle();
private:
};