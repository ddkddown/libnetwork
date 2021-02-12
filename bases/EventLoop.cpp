#include "EventLoop.h"
#include "Logger.h"

EventLoop::EventLoop():dispatcher_(this),quit_(false){
    if(-1 == pipe(fds_)) {
        LOG_ERR<<"create pipe failed!";
    }
    LOG_DEBUG<<"fds_[0] is "<<fds_[0]<<"fds_[1] is "<<fds_[1];

    Channel c(fds_[0], EPOLLIN, bind(&EventLoop::NotifyQuit, this, 
            std::placeholders::_1, std::placeholders::_2), nullptr, nullptr);
    channMap_.insert(pair<int, Channel>(fds_[0], c));
    dispatcher_.AddChannel(c);
}

EventLoop::~EventLoop() {
    close(fds_[0]);
    close(fds_[1]);
}

int EventLoop::Quit() {
    char a = 'a';
    write(fds_[1], &a, 1);
}

int EventLoop::NotifyQuit(int fd, void *data) {
    quit_ = true;
}

void EventLoop::AddChannel(Channel &c) {
    lock_guard<mutex> lck(m_);
    queueNode tmp = {c, ADD, 0};
    pendingQueue_.push(tmp);
}

void EventLoop::DelChannel(Channel &c) {
    lock_guard<mutex> lck(m_);
    queueNode tmp = {c, DELETE, 0};
    pendingQueue_.push(tmp);
}

void EventLoop::UpdateChannel(Channel &c) {
    lock_guard<mutex> lck(m_);
    queueNode tmp = {c, UPDATE, 0};
    pendingQueue_.push(tmp);
}

void EventLoop::EventActive(int fd, int event) {
    auto c = channMap_.find(fd);
    if(channMap_.end() == c) {
        LOG_WARN<<"map not contain fd:"<<fd;
        return;
    }

    queueNode tmp = {c->second, ACTIVE, event};
    pendingQueue_.push(tmp);
}

void EventLoop::Run() {
    while(!quit_) {
        dispatcher_.Dispatch();
        HandlePendingChannel();
    }
}

void EventLoop::HandlePendingChannel() {
    lock_guard<mutex> lck(m_);
    while(!pendingQueue_.empty()) {
        auto node = pendingQueue_.front();
        pendingQueue_.pop();

        //TODO 状态机修改
        switch (node.type)
        {
        case ACTIVE:
            if(node.event & EPOLLIN) {
                node.c.GetReadCall()(node.c.GetFd(), nullptr);
            }

            if(node.event & EPOLLOUT) {
                node.c.GetWriteCall()(node.c.GetFd(), nullptr);
            }
            break;
        case ADD:
            channMap_.insert(pair<int, Channel>(node.c.GetFd(), node.c));
            dispatcher_.AddChannel(node.c);
            break;
        case UPDATE:
            dispatcher_.UpdateChannel(node.c);
            break;
        case DELETE:
            channMap_.erase(channMap_.find(node.c.GetFd()));
            dispatcher_.DeleteChannel(node.c);
            break;
        default:
            LOG_INFO<<"unknown type: "<<node.type;
            break;
        }
    }
}