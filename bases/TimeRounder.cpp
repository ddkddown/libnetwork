#include "TimeRounder.h"
static int GetTimerFd() {
    //TODO
    return 0;
}
TimeRounder::TimeRounder(EventLoop *loop, int wheelSize):
            timerFd_(GetTimerFd()), index_(0),
            loop_(loop), timerChann_(loop, timerFd_),
            wheelSize_(wheelSize), wheel_(wheelSize_) {
    timerChann_.SetReadCallbk(&TimeRounder::HandleRead, this);
}

void TimeRounder::Run() {
    timerChann_.EnableRead();
}

void TimeRounder::AddTimer(TimeCallback cb, int interval, bool repeat) {
    //TODO 加锁?
    wheel_[(index_+interval)%wheelSize_].push_back(make_shared<Timer>(interval, repeat, cb));
}
void TimeRounder::HandleRead() {
    index_ = (++index_)%wheelSize_;
    for(auto &i : wheel_[index_]) {
        i->Run();
    }
}


