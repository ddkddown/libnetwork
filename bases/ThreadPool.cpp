#include "ThreadPool.h"
#include "Logger.h"

ThreadPool::ThreadPool(int size):size_(size), position_(0) {
    if(size_ <= 0) {
        LOG_INFO<<"input size:"<<size<<" make it default 1";
        size_ = 1;
    }

    for (int i = 0; i < size_; ++i) {
        pool_.push_back(make_shared<LoopThread>());
    }
}

ThreadPool::~ThreadPool() {}

EventLoop& ThreadPool::GetLoop() {
    //轮询(除了mainLoop)
    position_ %= size_;
    return pool_[position_++]->GetLoop(); 
}

EventLoop& ThreadPool::GetMainLoop() {
    return mainLoop_;
}

void ThreadPool::Run() {
    for (int i = 0; i < pool_.size(); ++i) {
        pool_[i]->run();
    }

    mainLoop_.Loop();
}