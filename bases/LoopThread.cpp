#include <assert.h>
#include "LoopThread.h"

LoopThread::LoopThread():t_(nullptr) {
    t_ = make_shared<thread>(bind(&LoopThread::run, this));
    assert(nullptr != t_);
}

LoopThread::~LoopThread() {
    loop_.Quit();
    if(t_->joinable()) t_->join();
}

void LoopThread::run() {
    loop_.Run();
}

EventLoop& LoopThread::GetLoop() {
        return loop_;
}