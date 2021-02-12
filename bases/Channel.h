#pragma once
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

using namespace std;
using EventReadCallback = function<int(int,void*)>;
using EventWriteCallback = function<int(int, void*)>;

class Channel {
public:
    Channel(int fd, int event, EventReadCallback readCall,
                EventWriteCallback writeCall, void *data):
                fd_(fd), event_(event), readCall_(readCall),
                writeCall_(writeCall), data_(data) {}
    
    Channel(const Channel& c) {
        fd_ = c.fd_;
        event_ = c.event_;
        readCall_ = c.readCall_;
        writeCall_ = c.writeCall_;
        data_ = c.data_;
    }

    Channel& operator = (const Channel &c) {
        fd_ = c.fd_;
        event_ = c.event_;
        readCall_ = c.readCall_;
        writeCall_ = c.writeCall_;
        data_ = c.data_;
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

    EventReadCallback GetReadCall() {
        return readCall_;
    }

    EventWriteCallback GetWriteCall() {
        return writeCall_;
    } 
private:
    int fd_;
    int event_;
    EventReadCallback readCall_;
    EventWriteCallback writeCall_;
    void *data_;
};