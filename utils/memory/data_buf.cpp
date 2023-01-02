#include <sys/ioctl.h>
#include <unistd.h>
#include <memory.h>
#include <cassert>

#include "data_buf.h"
#include "pr.h"

BufferBase::BufferBase() {}

BufferBase::~BufferBase() { clear(); }

const int BufferBase::length() const {
    return data_buf_ != nullptr ? data_buf_->length_ : 0;
}

void BufferBase::pop(int len) {
    assert(data_buf_ != nullptr && len <= data_buf_->length_);
    data_buf_->pop(len);
    if (data_buf_->length_ == 0) {
        MemPool::getInstance().retrieve(data_buf_);
        data_buf_ = nullptr;
    }
}

void BufferBase::clear() {
    if (data_buf_ != nullptr) {
        MemPool::getInstance().retrieve(data_buf_);
        data_buf_ = nullptr;
    }
}

int InputBuffer::readFromFd(int fd) {
    int need_read;
    if (ioctl(fd, FIONREAD, &need_read) == -1) {
        PR_ERROR("ioctl FIONREAD error\n");
        return -1;
    }

    if (data_buf_ == nullptr) {
        data_buf_ = MemPool::getInstance().allocChunk(need_read);
        if (data_buf == nullptr) {
            PR_INFO("no free buf for alloc\n");
            return -1;
        }
    }
    else {
        assert(data_buf_->head_ == 0);
        if (data_buf_->capacity_ - data_buf_->length_ < (int)need_read) {
            Chunk *new_buf = MemPool::getInstance().allocChunk(need_read+data_buf_->length_);
            if (new_buf == nullptr) {
                PR_INFO("no free buf for alloc\n");
                return -1;
            }
            new_buf->copy(data_buf_);
            MemPool::getInstance().retrieve(data_buf_);
            data_buf_ = new_buf;
        }
    }

    int already_read = 0;
    do {
        if (need_read == 0) already_read = read(fd, data_buf_->data_+data_buf_->length_, m4K);
        else already_read = read(fd, data_buf_->data_+data_buf_->length_, need_read);
    } while (already_read == -1 && std::errno == EINTR);
    if (already_read > 0) {
        if (need_read != 0) assert(already_read == need_read);
        data_buf_->length_ += already_read;
    }
    return already_read;
}

const char* InputBuffer::getFromBuf() const {
    return data_buf_ != nullptr ? data_buf_->data_ + data_buf_->head_ : nullptr;
}

void InputBuffer::adjust() {
    if (data_buf_ != nullptr) data_buf_->adjust();
}

int OutputBuffer::write2buf(const char* data, int len) {
    if (data_buf_ == nullptr) {
        data_buf_ = MemPool::getInstance().allocChunk(len);
        if (data_buf == nullptr) {
            PR_INFO("no free buf for alloc\n");
            return -1;
        }
    }
    else {
        assert(data_buf_->head_ == 0);
        if (data_buf_->capacity_ - data_buf_->length_ < len) {
            Chunk *new_buf = MemPool::getInstance().allocChunk(len+data_buf_->length_);
            if (new_buf == nullptr) {
                PR_INFO("no free buf for alloc\n");
                return -1;
            }
            new_buf->copy(data_buf_);
            MemPool::getInstance().retrieve(data_buf_);
            data_buf_ = new_buf;
        }
    }
    memcpy(data_buf_->data_ + data_buf_->length_, data, len);
    data_buf_->length_ += len;
    return 0;
}

int OutputBuffer::write2fd(int fd) {
    assert(data_buf_ != nullptr && data_buf_->head_ == 0);
    int already_write = 0;
    do {
        already_write = write(fd, data_buf_->data_, data_buf_->length_);
    } while (already_write == -1 && std::errno == EINTR);
    if (already_write > 0) {
        data_buf_->pop(already_write);
        data_buf_->adjust();
    }
    
    if (already_write == -1 && std::errno == EAGAIN) {
        already_write = 0;
    }

    return already_write;
}