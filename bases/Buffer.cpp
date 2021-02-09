#include "Buffer.h"
#include "Logger.h"

Buffer::Buffer():readIndex_(0),writeIndex_(0)
                ,size_(INIT_BUFFER_SIZE),data_(nullptr) {
    try {
        data_ = new char[size_];
    }
    catch(const std::exception& e) {
        LOG_ERR<<"init buffer failed!";
    }
    assert(data_ != nullptr);
}

Buffer::~Buffer() {
    if(data_) delete [] data_;
}

int Buffer::ReadSize() {
    return writeIndex_ - readIndex_;
}

int Buffer::WriteSize() {
    return size_ - writeIndex_;
}

int Buffer::Write
