#include "Channel.h"
#include "EventLoop.h"

bool Channel::CheckEventEnable(int event) {
    return event_ & event;
}

void Channel::UpdateEvent(int event) {
    event_ |= event;
    EventLoop *eventLoop = static_cast<EventLoop*>(data_);
    eventLoop->UpdateChannel(*this);
}

void Channel::DisableEvent(int event) {
    event_ &= (~event);
    EventLoop *eventLoop = static_cast<EventLoop*>(data_);
    eventLoop->UpdateChannel(*this);
}