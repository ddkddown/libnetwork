#pragma once
#include <iostream>
using namespace std;

class Acceptor {
public:
    Acceptor(int port);
    ~Acceptor();
    int GetFd();
private:
    enum {
        LISTENQ = 100
    };
    
    int port_;
    int fd_;
};