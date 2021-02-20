#pragma once
#include <iostream>
#include "Channel.h"
#include "EventLoop.h"
using namespace std;

class Acceptor : boost::noncopyable{
public:
    using NewConnectionCallBk = function<void(int)>;
    Acceptor(EventLoop *loop, int port);
    ~Acceptor();
    inline void SetNewConnCallBk(const NewConnectionCallBk &cb) {
        newConnCallBk_ = cb;
    }
    void Listen();
private:
    void HandleRead();
private:
    enum {
        LISTENQ = 100
    };
    int port_;
    int fd_;
    struct sockaddr_in cliaddr_;
    socklen_t cliLen_;
    NewConnectionCallBk newConnCallBk_;
    EventLoop *loop_;
    shared_ptr<Channel> acceptChannel_;
};