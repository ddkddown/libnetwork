#pragma once
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "Logger.h"

using namespace std;
using EventReadCallback = function<int(void*)>;
using EventWriteCallback = function<int(void*)>;

class Channel {
public:
    Channel(int fd, int event, EventReadCallback readCall = nullptr,
                EventWriteCallback writeCall = nullptr, void *data = nullptr):
                fd_(fd), event_(event), readCall_(move(readCall)),
                writeCall_(move(writeCall)), data_(data) {}
    
    Channel(const Channel& c) {
        fd_ = c.fd_;
        event_ = c.event_;
        data_ = c.data_;
        readCall_ = c.readCall_;
        writeCall_ = c.writeCall_;
    }

    Channel& operator=(const Channel &c) {
        if(&c == this){
            return *this;
        }

        fd_ = c.fd_;
        event_ = c.event_;
        readCall_ = c.readCall_;
        writeCall_ = c.writeCall_;
        data_ = c.data_;
        return *this;
    }

    ~Channel() {
    }

    bool CheckEventEnable(int event);

    void UpdateEvent(int event);
    
    void DisableEvent(int event);

    int GetFd() {
        return fd_;
    }

    int GetEvent() {
        return event_;
    }

    const EventReadCallback GetReadCall() {
        return readCall_;
    }

    const EventWriteCallback GetWriteCall() {
        return writeCall_;
    }

    void setReadCall(EventReadCallback cb)
    { readCall_ = std::move(cb); }
    void setWriteCallback(EventWriteCallback cb)
    { writeCall_ = std::move(cb); }
private:
    int fd_;
    int event_;
    EventReadCallback readCall_;
    EventWriteCallback writeCall_;
    void *data_;
};