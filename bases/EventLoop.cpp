#include <fcntl.h>
#include <errno.h>
#include "EventLoop.h"
#include "Logger.h"

static bool CheckFileAvaliable(int fd) {
    return !(-1 == fcntl(fd, F_GETFD, NULL) && errno == EBADF);
}

EventLoop::EventLoop():dispatcher_(this),quit_(false){
    if(-1 == pipe(fds_)) {
        LOG_ERR<<"create pipe failed!";
    }

    Channel c(fds_[0], EPOLLIN, bind(&EventLoop::NotifyQuit, this, 
            std::placeholders::_1, std::placeholders::_2), nullptr, nullptr);
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

int EventLoop::NotifyQuit(int fd, void *data) {
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
    pendingQueue_.push(tmp);
}

void EventLoop::AddAcceptor(Channel &c) {
    channMap_.insert(pair<int, Channel>(c.GetFd(), c));
    dispatcher_.AddChannel(c);
}

void EventLoop::DelChannel(Channel &c) {
    queueNode tmp = {c, DELETE, 0};
    pendingQueue_.push(tmp);
}

void EventLoop::UpdateChannel(Channel &c) {
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
    while(!pendingQueue_.empty()) {
        auto node = pendingQueue_.front();
        pendingQueue_.pop();

        LOG_DEBUG<<"node fd" <<node.c.GetFd()<<endl;
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

            if(!CheckFileAvaliable(node.c.GetFd())) {
                queueNode tmp = {node.c, DELETE, 0};
                pendingQueue_.push(tmp);
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