#pragma once
#include <iostream>
using namespace std;

//TODO 后续改为链式内存页管理
class Buffer {
public:
    Buffer();
    ~Buffer();
    int WriteSize();
    int ReadSize();
    int Write(char *data, int size);
    int Read(char *dst, int size);
    int ReaDataFromFd(int fd);
    int WriteDataToFd(int fd);
    void Flush();
private:
    enum {
        INIT_BUFFER_SIZE = 65536
    };

    char *data_;
    int readIndex_;
    int writeIndex_;
    int size_;
};



