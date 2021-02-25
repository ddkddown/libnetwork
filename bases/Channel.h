#pragma once
#include <iostream>
#include <functional>
#include <boost/noncopyable.hpp>
#include <memory>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "Logger.h"

class EventLoop;

using namespace std;
using EventCallback = function<void()>;
using ReadCallback = function<void()>;

class Channel : boost::noncopyable {
public:
    enum EVENT {
        NONE = 0,
        READ = EPOLLIN | EPOLLPRI, //EPOLLPRI 带外数据
        WRITE = EPOLLOUT
    };

    Channel(EventLoop *loop, int fd);
    ~Channel();

    void HandleEvent();
    inline void SetReadCallbk(const ReadCallback &cb) {
        readCall_ = cb;
    }
    inline void SetWriteCallbk(const EventCallback &cb) {
        writeCall_ = cb;
    }
    inline void SetCloseCallbk(const EventCallback &cb) {
        closeCall_ = cb;
    }
    inline void SetErrorCallbk(const EventCallback &cb) {
        errorCall_ = cb;
    }

    void Tie(const shared_ptr<void>&);

    int GetFd() {
        return fd_;
    }

    int GetEvent() {
        return event_;
    }

    void EnableRead() {
        event_ |= READ;
        Update();
    }

    void EnableWrite() {
        event_ |= WRITE;
        Update();
    }

    void DisableAll() {
        event_ = NONE;
        Update();
    }
    bool IsWriting() {
        return event_ & WRITE;
    }
    void SetEvent(int event) {
        event_ = event;
        Update();
    }

    inline void SetREvent(int revent) {
        revents_ = revent;
    }

    int GetIndex() {
        return index_;
    }

    void SetIndex(int idx) {
        index_ = idx;
    }

    EventLoop* GetLoop() {
        return loop_;
    }
    
private:
    void Update();
    void CheckHandleEvent();
private:
    EventLoop *loop_;
    const int fd_;
    int event_; //用户设置
    int revents_; //epoll返回
    int index_;

    weak_ptr<void> tie_; //用于保证owner对象存在, 使用weak_ptr不增加引用
    bool tied_;
    ReadCallback readCall_;
    EventCallback writeCall_;
    EventCallback closeCall_;
    EventCallback errorCall_;
};