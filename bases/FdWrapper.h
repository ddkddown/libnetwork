#pragma once
#include <iostream>
#include <memory>
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

class FdWrapper : public network::Nocopy, public std::enable_shared_from_this<FdWrapper> {
public:
    FdWrapper(int fd, int type, ReadHandler reader = nullptr,
                            WriteHandler writer = nullptr)
            :fd_(fd), type_(type), reader_(reader), writer_(writer) {
        assert(fd_ >= 0);
    }

    ~FdWrapper() {
        close(fd_);
    }
    
    int GetRawFd() {
        return fd_;
    }

    std::shared_ptr<FdWrapper> GetWrapper() {
        return shared_from_this();
    }

    ReadHandler GetReader() {
        return reader_;
    }

    WriteHandler GetWriter() {
        return writer_;
    }

    int GetEventType() {
        return type_;
    }
    
private:
    int fd_;
    ReadHandler reader_;
    WriteHandler writer_;
    int type_;
};
}