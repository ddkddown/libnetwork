#pragma once
#include <vector>
#include "LoopThread.h"

class ThreadPool {
public:
    ThreadPool(int size);
    ~ThreadPool();
    EventLoop& GetLoop();
    EventLoop& GetMainLoop();
private:
    int mainThread_;
    int size_;
    int position_;
    vector<LoopThread> pool_;
};