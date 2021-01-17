#pragma once
#include "TcpBase.h"

class TcpClient;
using SharedClient = std::shared_ptr<TcpClient>;
using CSharedClientRef = const std::shared_ptr<TcpClient>&;

class TcpClient : public TcpBase{
public:
    TcpClient(int fd)
            :TcpBase(fd){}
            
    virtual void ReadHandle() {
        LOG_DEBUG<<"got read event";
        CloseFd();
    }
    
    virtual void WriteHandle() {}
};