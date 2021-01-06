#pragma once
#include <iostream>
extern "C" {
#include <unistd.h>
}

using namespace std;

class EchoClient {
public:
    EchoClient(int sock)
            :sock_(sock){}
    ~EchoClient() {
        close(sock_);
    }

    ssize_t Read(int size, char *buf) {
        return read(sock_, buf, size);
    }

    ssize_t Write(int size, char *buf) {
        return write(sock_, buf, size);
    }

private:
    int sock_;
};
