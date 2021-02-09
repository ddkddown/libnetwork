#include "ThreadPool.h"
#include "Logger.h"

ThreadPool::ThreadPool(int size):size_(size), position_(1),
                                mainThread_(0) {
    if(size_ <= 0) {
        LOG_INFO<<"input size:"<<size<<" make it default 1";
        size_ = 1;
    }

    for (int i = 0; i < size_; ++i) {
        pool_.push_back(LoopThread());
    }
}

ThreadPool::~ThreadPool() {}

EventLoop& ThreadPool::GetLoop() {

    // 只有一个mainThread时返回mainLoop
    if(1 == size_) {
        return pool_[0].GetLoop();
    }

    //轮询(除了mainThread)
    position_ %= size_;
    if(0 == position_) {
        position_++;
    }

    return pool_[position_++].GetLoop(); 
}

EventLoop& ThreadPool::GetMainLoop() {
    return pool_[0].GetLoop();
}