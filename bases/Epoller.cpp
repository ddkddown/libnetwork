#include <string.h>
#include "Epoller.h"
#include "EventLoop.h"
#include "Logger.h"

Epoller::Epoller(void* loop):
                events_(InitEventSize),
                efd_(0),
                Poller(loop) {
    efd_ = epoll_create(1);
    if(-1 == efd_) {
        LOG_ERR<<"create epoll failed!"<<strerror(errno) ;
    }
}

Epoller::~Epoller() {
    close(efd_);
}

void Epoller::DeleteChannel(Channel *c) {
    int fd = c->GetFd();
    auto n = channels_.erase(fd);
    if(1 != n) {
        LOG_ERR<<"channel erase error, n:"<<n<<endl;
    }
    Update(EPOLL_CTL_DEL, c);

}

void Epoller::UpdateChannel(Channel *c) {
    
    // 当io线程阻塞在epoll_wait时，进行epoll_ctl增加fd操作是允许的, 但关闭是会导致未定义行为
    //详情见man 手册 参考: https://man7.org/linux/man-pages/man2/epoll_wait.2.html
    if(c->GetIndex() < 0) {
        //添加
        c->SetIndex(1);
        channels_[c->GetFd()] = c;
        Update(EPOLL_CTL_ADD, c);
    } else {
        Update(EPOLL_CTL_MOD, c);
    }
}

void Epoller::Dispatch(ChanneList *activeChannels) {
    auto num = epoll_wait(efd_, &*events_.begin(),
                static_cast<int>(events_.size()), -1);
    if(num <= 0) {
        LOG_INFO<<"num: "<<num<<" <= 0"<<endl;
        return;
    }

    for(int i = 0; i < num; ++i) {
        Channel *channel = static_cast<Channel*>(events_[i].data.ptr);
        int fd = channel->GetFd();
        ChannelMap::const_iterator it = channels_.find(fd);
        assert(it != channels_.end());
        channel->SetREvent(events_[i].events);
        activeChannels->push_back(channel);
    }

    if(static_cast<size_t>(num) == events_.size()) {
        events_.resize(events_.size()*2);
    }
}

void Epoller::Update(int op, Channel *channel) {
    struct epoll_event event;
    bzero(&event, sizeof event);
    event.events = channel->GetEvent();
    event.data.ptr = channel;
    if(0 > epoll_ctl(efd_, op, channel->GetFd(), &event)) {
        LOG_ERR<<"epoll_ctl op "<<op<<" failed!"<<endl;
    }
}