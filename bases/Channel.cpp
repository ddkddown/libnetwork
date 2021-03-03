#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd)
        :loop_(loop),
         fd_(fd),
         event_(NONE),
         revents_(NONE),
         index_(-1),
         tied_(false){}

Channel::~Channel() {
}

void Channel::Tie(const shared_ptr<void> &obj) {
    tie_ = obj;
    tied_ = true;
}

void Channel::Update() {
    loop_->UpdateChannel(this);
}

void Channel::CheckHandleEvent() {
    shared_ptr<void> guard; //此guard用于保证owner存在
    if(tied_) {
        guard = tie_.lock();
        if(guard) {
            HandleEvent();
            return;
        }
    }

    LOG_WARN<<"Check Handle Event failed, tied_: "<<tied_<<endl;
}

void Channel::HandleEvent() {
    if(revents_ & EPOLLERR) {
        if(readCall_) readCall_();
    } else if(revents_ & EPOLLHUP && !(revents_ & EPOLLRDHUP)) {
        if(readCall_) readCall_();
    } else {
        if (revents_ & EPOLLIN) {
            if(readCall_) readCall_();
        }

        if (revents_ & EPOLLOUT) {
            if(writeCall_) writeCall_();
        }

        if(revents_ & EPOLLRDHUP) {
            if(closeCall_) closeCall_();
        }
    }
}