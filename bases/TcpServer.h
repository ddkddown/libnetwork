#pragma once
#include <signal.h>
#include "TcpConnection.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "ThreadPool.h"
#include "Time.h"

//TODO 暴露出定制接口，例如clientRead, clientWrite, CloseHandler

class TcpServer {
public:
    TcpServer(int port, int poolSize, const ConnetionCallBk &cn,
            const MessageCallBk &ms, const WriteCompleteBk &wr);
    virtual ~TcpServer();
    void Start();
    void Quit();

    inline void SetConnectionCallBk(const ConnetionCallBk &cb) {
        connectionCallBk_ = cb;
    }

    inline void SetMessageCallBk(const MessageCallBk &cb) {
        messageCallBk_ = cb;
    }

    inline void SetWriteCompleteCallBk(const WriteCompleteBk &cb) {
        writeCompleteBk_ = cb;
    }
private:
    void NewConn(int sockfd);
    void RemoveConnection(const TcpConnectionPtr& conn);
    void RemoveConnectionInLoop(const TcpConnectionPtr& conn);

private:
    using ConnectionMap = map<Id, TcpConnectionPtr>;
    ConnetionCallBk connectionCallBk_;
    MessageCallBk messageCallBk_;
    WriteCompleteBk writeCompleteBk_;
    ThreadPool pool_;
    shared_ptr<Acceptor> accpt_;
    ConnectionMap connections_;
    bool started_;
};