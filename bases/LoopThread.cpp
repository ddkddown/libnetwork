#include <assert.h>
#include "LoopThread.h"
#include "Logger.h"

LoopThread::LoopThread():t_(nullptr) {
}

LoopThread::~LoopThread() {
    loop_.Quit();
    if(t_->joinable()) t_->join();
}
void LoopThread::ThreadFunc() {
    do
    {
        loop_.Loop();
    } while (0);
}
void LoopThread::run() {
    t_ = make_shared<thread>(&LoopThread::ThreadFunc, this);
}

EventLoop& LoopThread::GetLoop() {
        return loop_;
}