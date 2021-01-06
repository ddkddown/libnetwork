#pragma once
#include "TcpBase.h"

class TcpClient;
using SharedClient = std::shared_ptr<TcpClient>;
using CSharedClientRef = const std::shared_ptr<TcpClient>&;

class TcpClient : public TcpBase{
public:
    TcpClient(int port)
            :TcpBase(port){}

    virtual void ReadHandle() = 0;
    virtual void WriteHandle() = 0;
};