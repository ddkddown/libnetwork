#pragma once
#include <signal.h>
#include "TcpConnection.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "ThreadPool.h"

//TODO 暴露出定制接口，例如clientRead, clientWrite, CloseHandler

class TcpServer {
public:
    TcpServer(int port, int poolSize);
    virtual ~TcpServer();
    void Start();
protected:
    void SetReader(EventReadCallback read);
    void SetWriter(EventWriteCallback write);
    int GetAcceptFd();
private:
    int HandleAcceptor(int fd, void *data);
private:
    class IgnoreSig {
    public:
        IgnoreSig() {
            ::signal(SIGPIPE, SIG_IGN);
        }        
    };
    static IgnoreSig initSig_;
    EventReadCallback readHandler_;
    EventWriteCallback writeHandler_;
    Acceptor accpt_;
    ThreadPool pool_;
};