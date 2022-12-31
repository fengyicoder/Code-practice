#pragma once

#include <stdlib.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <assert.h>
#include <stdexcept>

#include "pr.h"

template <typename T>
class BufferQueue {
public:
    BufferQueue(): array_(new T[capacity_]) {}
    explicit BufferQueue(int max_size, bool debug=false) {
        if (max_size <= 0) {
            PR_ERROR("max_size is illegal!\n");
            throw std::invalid_argument("max_size is illegal!");
        }

        debug_ = debug;
        capacity_ = max_size;
        array_ = new T[capacity_];
    }
    ~BufferQueue() {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (array_) delete [] array_;
    }
    void clear() {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (debug_) {
            rcnt_ = 0;
            wcnt_ = 0;
        }
        first_ = -1;
        last_ = -1;
        size_ = 0;
    }
    int getRCnt() const {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (debug_) return rcnt_;
        PR_WARN("this method is invalid when q_debug is false!\n");
        return -1;
    }
    int getWCnt() const {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (debug_) return wcnt_;
        PR_WARN("this method is invalid when q_debug is false!\n");
        return -1;
    }
    int getSize() const {
        std::lock_guard<std::mutex> mtx(mutex_);
        assert(size_ >= 0 && size_ <= capacity_);
        return size_;
    }
    bool isFull() const {
        std::lock_guard<std::mutex> mtx(mutex_);
        return size_ == capacity_ ? true : false;
    }
    bool isEmpty() const {
        std::lock_guard<std::mutex> mtx(mutex_);
        return size_ == 0 ? true : false;
    }
    int getCapacity() const {
        return capacity_;
    }
    bool front(T& value) {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (size_ == 0) return false;
        value = array_[first_+1];
        return true;
    }
    bool back(T& value) {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (size_ == 0) return false;
        value = array_[last_];
        return true;
    }
    bool push(const T& item) {
        std::lock_guard<std::mutex> mtx(mutex_);
        if (size_ >= capacity_) {
            cond_.notify_all();
            return false;
        }
        last_ = (last_ + 1) % capacity_;
        array_[last_] = item;
        size_++;
        if (debug_) wcnt_++;
        cond_.notify_all();
    }
    void notify() {
        cond_.notify_all();
    }
    bool pop(T& item) {
        std::unique_lock<std::mutex> mtx(mutex_);
        if (size_ <= 0) {
            cond_.wait(mtx);
            if (size_ <= 0) return false;
        }
        first_ = (first_+1) % capacity_;
        item = array_[first_];
        size_--;
        if (debug_) rcnt_++;
        return true;
    }
    bool pop(T& item, int ms_timeout) {
        std::unique_lock<std::mutex> mtx(mutex_);
        if (size_ <= 0) {
            if (cond_.wait_for(mtx, std::chrono::milliseconds(ms_timeout)) == std::cv_status::timeout)
                return false;
        }
        if (size_ <= 0) return false;
        first_ = (first_+1) % capacity_;
        item = array_[first_];
        size_--;
        if (debug_) rcnt_++;
        return true;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    T *array_;
    long long rcnt_ = 0;
    long long wcnt_ = 0;
    bool debug_ = false;
    int first_ = -1;
    int last_ = -1;
    int size_ = 0;
    int capacity_ = 1000;
};
