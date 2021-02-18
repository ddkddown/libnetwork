#include <assert.h>
#include "LoopThread.h"
#include "Logger.h"

LoopThread::LoopThread():t_(nullptr) {
}

LoopThread::~LoopThread() {
    loop_.Quit();
    if(t_->joinable()) t_->join();
}

void LoopThread::run() {
    t_ = make_shared<thread>(bind(&EventLoop::Run, &loop_));
}

EventLoop& LoopThread::GetLoop() {
        return loop_;
}