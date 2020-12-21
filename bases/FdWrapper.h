#include <iostream>
#include <functional>

extern "C" {
#include <assert.h>
#include <unistd.h>
}

#include "Nocopyable.h"
using namespace network;

namespace {

using WriteHandler = std::function<void()>;
using ReadHandler = std::function<void()>;

class FdWrapper : network::Nocopy {
public:
    FdWrapper(int fd, ReadHandler reader = nullptr,
                    WriteHandler writer = nullptr)
            :fd_(fd), reader_(reader), writer_(writer) {
        assert(fd_ >= 0);
    }

    ~FdWrapper() {
        close(fd_);
    }
    
    int GetRawFd() {
        return fd_;
    }

    ReadHandler GetReader() {
        return reader_;
    }

    WriteHandler GetWriter() {
        return writer_;
    }
    
private:
    int fd_;
    ReadHandler reader_;
    WriteHandler writer_;
};
}