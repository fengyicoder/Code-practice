#include "thread_pool.h"

ThreadPool::ThreadPool(unsigned size): run_{true}, num_{0} {
    assert(size <= THREAD_COUNT);
    addThread(size);
}

ThreadPool::~ThreadPool() {
    run_ = false;
    cond_mutex_.notify_all();
    for (std::thread& thread: threads_) {
        if (thread.joinable()) thread.join();
    }
}

void ThreadPool::addThread(unsigned size) {
    for (; threads_.size() < THREAD_COUNT && size > 0; size--) {
        threads_.emplace_back([this]{
            while (run_)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cond_mutex_.wait(lock, [this]{
                        return !run_ || !tasks_.empty();
                    });
                    if (!run_ && tasks_.empty()) return;
                    task = move(tasks_.front());
                    tasks_.pop();
                }
                num_--;
                task();
                num_++;
            }
        });
        num_++;
    }
}
