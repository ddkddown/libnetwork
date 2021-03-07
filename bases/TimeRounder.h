#pragma once
#include <vector>
#include <list>
#include "Timer.h"
#include "Channel.h"

class EventLoop;

class TimeRounder : public boost::noncopyable {
public:
    TimeRounder(EventLoop *loop, int wheelSize);
    ~TimeRounder();
    void Run();
    void AddTimer(TimeCallback cb, int interval, bool repeat);
private:
    void HandleRead();
private:
    int timerFd_;
    int index_;
    int wheelSize_;
    EventLoop *loop_;
    Channel timerChann_;
    vector<list<SharedTimerPtr>> wheel_;
};