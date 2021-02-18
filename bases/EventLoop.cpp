#include <fcntl.h>
#include <errno.h>
#include "EventLoop.h"
#include "Logger.h"

EventLoop::EventLoop():dispatcher_(this),quit_(false){
    if(-1 == pipe(fds_)) {
        LOG_ERR<<"create pipe failed!";
    }

    Channel c(fds_[0], EPOLLIN, bind(&EventLoop::NotifyQuit, this, 
            std::placeholders::_1), nullptr, nullptr);
    channMap_.insert(pair<int, Channel>(fds_[0], c));
    dispatcher_.AddChannel(c);
}

EventLoop::~EventLoop() {
    for(auto i : channMap_) {
        close(i.first);
    }

    close(fds_[0]);
    close(fds_[1]);
}

int EventLoop::Quit() {
    char a = 'q';
    write(fds_[1], &a, 1);
}

int EventLoop::NotifyQuit(void *data) {
    char c;
    read(fds_[0], &c, 1);
    if('q' == c) {
        quit_ = true;
    }
}

void EventLoop::AddChannel(Channel &c) {
    char a = 'a';
    write(fds_[1], &a, 1);
    queueNode tmp = {c, ADD, 0};
    PushNode(tmp);
}

void EventLoop::AddAcceptor(Channel &c) {
    channMap_.insert(pair<int, Channel>(c.GetFd(), c));
    dispatcher_.AddChannel(c);
}

void EventLoop::DelChannel(Channel &c) {
    queueNode tmp = {c, DELETE, 0};
    PushNode(tmp);
}

void EventLoop::UpdateChannel(Channel &c) {
    queueNode tmp = {c, UPDATE, 0};
    PushNode(tmp);
}

void EventLoop::EventActive(int fd, int event) {
    auto c = channMap_.find(fd);
    if(channMap_.end() == c) {
        LOG_WARN<<"map not contain fd:"<<fd;
        return;
    }

    queueNode tmp = {c->second, ACTIVE, event};
    PushNode(tmp);
}

void EventLoop::Run() {
    LOG_DEBUG<<"eventLoop addr: "<<this<<" thread id"<<this_thread::get_id()<<endl;

    while(!quit_) {
        dispatcher_.Dispatch();
        HandlePendingChannel();
    }
}

void EventLoop::HandlePendingChannel() {
    while(!pendingQueue_.empty()) {
        LOG_DEBUG<<"pendingQueue_ size"<<pendingQueue_.size()<<endl;
        auto tmp = pendingQueue_.front();
        auto node = tmp;
        pendingQueue_.pop();

        LOG_DEBUG<<"node fd" <<node.c_.GetFd()<<endl;
        //TODO 状态机修改
        switch (node.type_)
        {
        case ACTIVE:
            if(node.event_ & EPOLLIN) {
                node.c_.GetReadCall()(nullptr);
            }

            if(node.event_ & EPOLLOUT) {
                node.c_.GetWriteCall()(nullptr);
            }
            break;
        case ADD:
            channMap_.insert(pair<int, Channel>(node.c_.GetFd(), node.c_));
            dispatcher_.AddChannel(node.c_);
            break;
        case UPDATE:
            dispatcher_.UpdateChannel(node.c_);
            break;
        case DELETE:
            channMap_.erase(channMap_.find(node.c_.GetFd()));
            dispatcher_.DeleteChannel(node.c_);
            break;
        default:
            LOG_INFO<<"unknown type: "<<node.type_;
            break;
        }
    }
}