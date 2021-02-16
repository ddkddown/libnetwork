#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "Buffer.h"
Buffer::Buffer():buff_(INITIAL_SIZE) {}

Buffer::~Buffer(){}

int Buffer::ReadFromFd(int fd) {
    char tmp[65536] = {0};
    int n = recv(fd, tmp, 65536, 0);
    if(n > 0) {
        if((buff_.capacity() - buff_.size()) < n) {
            buff_.resize(buff_.size() + n);
        }

        memcpy(&*(buff_.begin()+buff_.size()), tmp, n);
    }

    return n;
}

void Buffer::SendToFd(int fd) {
    if(buff_.empty()) {
        return;
    }

    send(fd, &*buff_.begin(), buff_.size(), 0);
}

void Buffer::AppenData(const char *data, int len) {
    if((buff_.capacity() - buff_.size()) < len) {
        buff_.resize(buff_.size() + len);
    }

    memcpy(&*(buff_.begin()+buff_.size()), data, len);
}

int Buffer::GetData(char *dst, int len) {
    if(len > buff_.size()) {
        len = buff_.size();
    }

    memcpy(dst, &*buff_.begin(), len);
    buff_.erase(buff_.begin(), buff_.begin()+len);
    
    return len;
}