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

    void SetHeartBeat(bool state) {
        heartBeat_ = state;
    }

    void SetHeartBeatNum(int num) {
        heartBeatNum_ = num;
    }

private:
    void NewConn(int sockfd);
    void RemoveConnection(const TcpConnectionPtr& conn);
    void RemoveConnectionInLoop(const TcpConnectionPtr& conn);
    void DefaultHeartBeatHandler(const TcpConnectionPtr& conn);

private:
    using ConnectionMap = map<Id, TcpConnectionPtr>;
    ConnetionCallBk connectionCallBk_;
    MessageCallBk messageCallBk_;
    WriteCompleteBk writeCompleteBk_;
    HeartBeatCallBk heartBeatCallBk_;
    ThreadPool pool_;
    shared_ptr<Acceptor> accpt_;
    ConnectionMap connections_;
    bool started_;
    bool heartBeat_;
    int heartBeatNum_; //心跳包超时时间
};