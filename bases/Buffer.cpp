#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "Buffer.h"
#include "Logger.h"
Buffer::Buffer():buff_(INITIAL_SIZE), writeIndex_(0) {}

Buffer::~Buffer(){}

int Buffer::ReadFromFd(int fd) {
    CheckFreeSpace();
    int n = recv(fd, BeginWrite(), FreeSpace(), 0);
    writeIndex_ += n;
    return n;
}

void Buffer::SendToFd(int fd) {
    if(buff_.empty()) {
        return;
    }

    int n = send(fd, Begin(), writeIndex_, 0);
    writeIndex_ -= n;
}

void Buffer::AppenData(const char *data, int len) {
    CheckFreeSpace(len);
    memcpy(BeginWrite(), data, len);
}

int Buffer::GetData(char *dst, int len) {
    if(len > writeIndex_) {
        len = writeIndex_;
    }

    memcpy(dst, Begin(), len);
    writeIndex_ -= len;
    
    return len;
}