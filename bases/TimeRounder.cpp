#include <sys/timerfd.h>
#include <signal.h>
#include "TimeRounder.h"

static int GetTimerFd() {
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
    assert(timerfd >= 0);
    return timerfd;
}

TimeRounder::TimeRounder(EventLoop *loop, int wheelSize):
            timerFd_(GetTimerFd()), index_(0),
            loop_(loop), 
            timerChann_(loop, timerFd_),
            wheelSize_(wheelSize), wheel_(wheelSize_) {
    timerChann_.SetReadCallbk(bind(&TimeRounder::HandleRead, this));
}

TimeRounder::~TimeRounder() {}

void TimeRounder::Run() {
    //TODO set timer
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


