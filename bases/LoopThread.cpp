#include <memory>
#include <assert.h>
#include "LoopThread.h"
#include "Logger.h"
#include "Epoller.h"

LoopThread::LoopThread():t_(nullptr) {
}

LoopThread::~LoopThread() {
    Quit();
}

void LoopThread::Quit() {
    loop_.Quit();
    if(t_->joinable()) t_->join();
}
void LoopThread::ThreadFunc() {
    loop_.SetPoller(std::make_shared<Epoller>(&loop_));
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