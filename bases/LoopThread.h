#pragma once
#include <memory>
#include "EventLoop.h"

class LoopThread {
public:
    LoopThread();
    ~LoopThread();
    EventLoop& GetLoop();
    void run();
private:
    // loop_必须在t_调用前构造
    EventLoop loop_;
    shared_ptr<thread> t_;
};