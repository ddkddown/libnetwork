#pragma once
#include <functional>
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <uuid/uuid.h>
#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "Time.h"

class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using ConnetionCallBk = function<void(const TcpConnectionPtr&)>;
using CloseCallBk = function<void(const TcpConnectionPtr&)>;
using WriteCompleteBk = function<void(const TcpConnectionPtr&)>;
using MessageCallBk = function<void(const TcpConnectionPtr&, Buffer*)>;
using Id = string;

class TcpConnection : public enable_shared_from_this<TcpConnection>,
                    boost::noncopyable {
public:
    TcpConnection(EventLoop *loop, int sockfd, Id id, TIMESTAMP now);

    virtual ~TcpConnection();

    EventLoop* GetLoop() {
        return loop_;
    }

    bool Connected() {
        return state_ == CONNECTED;
    }

    void Send(Buffer *message);

    void ShutDown();

    inline void SetConnectionCallBk(const ConnetionCallBk &cb) {
        connectionCallBk_ = cb;
    } 

    inline void SetMessageCallBk(const MessageCallBk &cb) {
        messageCallBk_ = cb;
    }

    inline void SetWriteCompleteCallBk(const WriteCompleteBk &cb) {
        writeCallBk_ = cb;
    }

    inline void SetCloseCallBk(const CloseCallBk &cb) {
        closeCallBk_ = cb;
    }

    void ConnectEstablished(); //创建连接时调用

    void ConnectDestroyed();
    int GetFd() {
        return fd_;
    }

    Id GetId() {
        return id_;
    }

    ELAPSED GetDuraion() {
        return chrono::system_clock::now() - now_;
    }

    void SetTimeStamp(const TIMESTAMP &time) {
        now_ = time;
    }
private:
    enum States {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        DISCONNECTING
    };
    void HandleRead();
    void HandleWrite();
    void HandleClose();
    void HandleError();
    void SendInLoop(const string &data);
    void ShutdownInLoop();
    void SetState(States s) {
        state_ = s;
    }

private:
    int fd_;
    Id id_;
    shared_ptr<Channel> channel_;
    EventLoop *loop_; //TODO weak_ptr?
    States state_;
    ConnetionCallBk connectionCallBk_;
    MessageCallBk messageCallBk_;
    WriteCompleteBk writeCallBk_;
    CloseCallBk closeCallBk_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    TIMESTAMP now_;
};
