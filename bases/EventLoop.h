#pragma once
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <unistd.h>
#include "Channel.h"
#include "EventDispatch.h"

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void AddChannel(Channel &c);
    void UpdateChannel(Channel &c);
    void DelChannel(Channel &c);
    void AddAcceptor(Channel &c);
    void Run();
    int Quit();
private:
    enum {
        ADD = 0,
        UPDATE,
        DELETE,
        ACTIVE
    };

    struct queueNode{
        Channel c_;
        int type_;
        int event_;
        queueNode(Channel c, int type,
                    int event):c_(c),type_(type),
                    event_(event){}

        queueNode(const queueNode& n):c_(n.c_),
                                    type_(n.type_),
                                    event_(n.event_) {}

        queueNode& operator=(const queueNode& n) {
            if(this == &n) {
                return *this;
            }

            c_ = n.c_;
            type_ = n.type_;
            event_ = n.event_;
            return *this;
        }
    };
private:
    void EventActive(int fd, int event);
    void HandlePendingChannel();
    int NotifyQuit(void *data);
    void PushNode(const queueNode &tmp) {
        lock_guard<mutex> lk(m_);
        pendingQueue_.push(tmp);
    }

private:

    bool quit_;
    int isHandlePending_;
    thread::id ownerThread_;
    int fds_[2];
    queue<queueNode> pendingQueue_;
    map<int, Channel> channMap_;
    EventDispatch dispatcher_;
    mutex m_;//TODO shared?
    friend class EventDispatch;
};