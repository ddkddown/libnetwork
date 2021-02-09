#pragma once
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

using namespace std;
typedef int (*EventReadCallback)(void *data = nullptr);
typedef int (*EventWriteCallback)(void *data = nullptr);

class Channel {
public:
    Channel(int fd, int event, function<EventReadCallback> readCall,
                function<EventWriteCallback> writeCall, void *data):
                fd_(fd), event_(event), readCall_(readCall),
                writeCall_(writeCall), data_(data) {}
    
    ~Channel() {
        //对缓冲区数据进行清空
        shutdown(fd_, SHUT_RDWR);
        close(fd_);
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

    function<EventReadCallback> GetReadCall() {
        return readCall_;
    }

    function<EventWriteCallback> GetWriteCall() {
        return writeCall_;
    } 
private:
    int fd_;
    int event_;
    function<EventReadCallback> readCall_;
    function<EventWriteCallback> writeCall_;
    void *data_;
};