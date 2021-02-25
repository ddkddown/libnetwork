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
    if(revents_ & EPOLLOUT) {
        if(writeCall_) writeCall_();
    }

    //rst会触发EPOLLHUP(读写端关闭)
    if(revents_ & EPOLLHUP) {
        LOG_WARN<<"handle EPOLLHUP"<<endl;
        if(closeCall_) closeCall_();
        return;
    }

    //文件描述符发生错误会触发EPOLLERR
    if(revents_ & EPOLLERR) {
        LOG_WARN<<"handle EPOLLERR"<<endl;
        if(errorCall_) errorCall_();
        return;
    }

    //EPOLLRDHUP(读端关闭)
    if(revents_ & (EPOLLIN|EPOLLPRI|EPOLLRDHUP)) {
        if(readCall_) readCall_();
        return;
    }
}