#pragma once;
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
    void Run();
    int Quit();
private:
    void EventActive(int fd, int event);
    void HandlePendingChannel();
    int NotifyQuit(void *data);
private:
    enum {
        ADD = 0,
        UPDATE,
        DELETE,
        ACTIVE
    };

    struct queueNode{
        Channel c;
        int type;
        int event;
    };

    bool quit_
    int isHandlePending_;
    thread::id ownerThread_;
    int fds_[2];
    mutex m_;
    queue<queueNode> pendingQueue_;
    map<int, Channel> channMap_;
    EventDispatch dispatcher_;

    friend class EventDispatch;
};