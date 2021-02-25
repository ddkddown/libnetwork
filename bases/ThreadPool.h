#pragma once
#include <vector>
#include <memory>
#include "LoopThread.h"

class ThreadPool {
public:
    ThreadPool(int size);
    ~ThreadPool();
    EventLoop& GetLoop();
    EventLoop& GetMainLoop();
    void Run();
    void Quit();
private:
    int size_;
    int position_;
    EventLoop mainLoop_;
    vector<shared_ptr<LoopThread>> pool_;
};