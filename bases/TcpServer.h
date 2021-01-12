#pragma once
#include "TcpBase.h"

class TcpServer;
using SharedServer = std::shared_ptr<TcpServer>;
using CSharedServerRef = const std::shared_ptr<TcpServer>&;

class TcpServer : public TcpBase{
public:
    TcpServer(int port, int lSize)
            :TcpBase(port, ALL),
             lSize_(lSize) {
        sock_.Bind();
        sock_.Listen(lSize_);
    }

    int Accept();

    virtual void ReadHandle() = 0;
    virtual void WriteHandle() = 0;
private:
    int lSize_;
};