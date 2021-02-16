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
    void SetReader(ReadCompleteCallBk read);
    void SetWriter(WriteCompleteCallBk write);
    int GetAcceptFd();
private:
    int HandleAcceptor(void *data);
private:
    class IgnoreSig {
    public:
        IgnoreSig() {
            ::signal(SIGPIPE, SIG_IGN);
        }        
    };
    static IgnoreSig initSig_;
    ReadCompleteCallBk readHandler_;
    WriteCompleteCallBk writeHandler_;
    Acceptor accpt_;
    ThreadPool pool_;
};