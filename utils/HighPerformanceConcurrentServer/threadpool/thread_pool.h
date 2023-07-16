#pragma once

#include <queue>
#include <vector>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>
#include <assert.h>

using Task = std::function<void()>;

class ThreadPool {
public:
    const unsigned THREAD_COUNT = std::thread::hardware_concurrency();

    ThreadPool(unsigned size = 4);
    ~ThreadPool();
    void addThread(unsigned size);
    template<typename F, typename... Args>
    decltype(auto) submit(F&& f, Args&&... args) {
        if (!run_) throw std::runtime_error("submit task on threadpool has been stopped!");
        using return_type = typename std::result_of_t<F(Args...)>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace([task](){
                (*task)();
            });
        }
        cond_mutex_.notify_one();
        return res;
    }

    int idlThreadCnt() { return num_; }
    int threadCnt() { return threads_.size(); }


private:
    std::queue<Task> tasks_;
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable cond_mutex_;
    std::atomic<bool> run_;
    std::atomic<int> num_;
};

