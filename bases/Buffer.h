//减少recv，send调用，尽量读取数据，防止send阻塞
#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Buffer {
enum {
    MIN_SIZE = 1024,
    INITIAL_SIZE = 4096
};
public:
    Buffer();
    ~Buffer();
    char* Begin() {
        return &*buff_.begin();
    }

    char* BeginWrite() {
        return Begin()+writeIndex_;
    }

    int FreeSpace() {
        return buff_.size() - writeIndex_;
    }
    int ReadFromFd(int fd);
    void SendToFd(int fd);
    void AppenData(const char *data, int len);
    int GetData(char *dst, int len);
private:
    void CheckFreeSpace(int len = MIN_SIZE) {
        if(FreeSpace() < len) {
            buff_.resize(buff_.size()+len, 0);
        }
    }

private:
    vector<char> buff_;
    int writeIndex_;
};