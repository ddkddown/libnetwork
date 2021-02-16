//减少recv，send调用，尽量读取数据，防止send阻塞
#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Buffer {
enum {
    INITIAL_SIZE = 1024
};
public:
    Buffer();
    ~Buffer();
    int ReadFromFd(int fd);
    void SendToFd(int fd);
    void AppenData(const char *data, int len);
    int GetData(char *dst, int len);

private:
    vector<char> buff_;
};