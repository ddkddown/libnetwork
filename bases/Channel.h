#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>

using namespace std;
typedef int (*EventReadCallback)(void *data);
typedef int (*EventWriteCallback)(void *data);

class Channel {
public:
    Channel(int fd, int event, EventReadCallback readCall,
                EventWriteCallback writeCall, void *data):
                fd_(fd), event_(event), readCall_(readCall),
                writeCall_(writeCall), data_(data) {}
    
    ~Channel() {
        close(fd_);
    }

    bool CheckEventEnable(int event);

    void UpdateEvent(int event);
    
    void DisableEvent(int event);
private:
    int fd_;
    int event_;
    EventReadCallback readCall_;
    EventWriteCallback writeCall_;
    void *data_;
};