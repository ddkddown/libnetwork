#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>
using namespace std;
class Timer;
using TimeCallback = function<void()>;
using SharedTimerPtr = shared_ptr<Timer>;

class Timer : boost::noncopyable {
public:
    Timer(int interval, bool repeat,
            TimeCallback cb):interval_(interval),
            repeat_(repeat),callback_(cb) {}
    
    bool Repeat() {
        return repeat_;
    }

    int Interval() {
        return interval_;
    }

    void Run() {
        callback_();
    }

private:
    const int interval_;
    const bool repeat_;
    const TimeCallback callback_;
};