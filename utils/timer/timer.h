#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>

#include "../threadpool/thread_pool.h"
#include "hash_map.h"

#define DEFAULT_TIMER_THREAD_POOL_SIZE 2

class Timer {
public:
    struct TimerNode {
        std::chrono::time_point<std::chrono::high_resolution_clock> tm_point;
        std::function<void()> callback;
        int repeated_id;
        int tm_id;
        int tm_repeated_num;
        bool tm_is_period{false};
        bool tm_is_repeated{false};
        std::chrono::milliseconds tm_period;
        bool operator<(const TimerNode& b) const {
            return tm_point > b.tm_point;
        }
    };

    enum class IdState{ Running = 0 };

    Timer(): tm_thread_pool_(DEFAULT_TIMER_THREAD_POOL_SIZE) {
        tm_id_.store(0);
        tm_running_.store(true);
    }

    ~Timer() {
        tm_running_.store(false);
        tm_cond_.notify_all();
        if (tm_tick_thread_.joinable()) {
            tm_tick_thread_.join();
        }
    }

    void run() {
        tm_tick_thread_ = std::thread([this](){ runLocal(); });
    }

    bool isAvailable() {
        return tm_thread_pool_.idlThreadCnt() >= 0;
    }

    int size() { return tm_queue_.size(); }

    template<typename F, typename... Args>
    int runAfter(int ms_time, bool is_period, F&& f, Args&&... args) {
        TimerNode s;
        s.tm_id = tm_id_.fetch_add(1);
        tm_id_state_map_.emplace(s.tm_id, IdState::Running);
        s.tm_is_period = is_period;
        s.tm_period = std::chrono::milliseconds(ms_time);
        s.tm_point = std::chrono::high_resolution_clock::now() + s.tm_period;
        s.callback = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(tm_mutex_);
        tm_queue_.push(s);
        tm_cond_.notify_all();
        return s.tm_id;
    }

    template<typename F, typename... Args>
    int runAt(const std::chrono::time_point<std::chrono::high_resolution_clock>& time_point, F&& f, Args&&... args) {
        TimerNode s;
        s.tm_id = tm_id_.fetch_add(1);
        tm_id_state_map_.emplace(s.tm_id, IdState::Running);
        s.tm_is_period = false;
        s.tm_point = time_point;
        s.callback = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(tm_mutex_);
        tm_queue_.push(s);
        tm_cond_.notify_all();
        return s.tm_id;
    }

    template<typename F, typename... Args>
    int runRepeated(int ms_time, int repeated_num, F&& f, Args&&... args) {
        TimerNode s;
        s.tm_id = tm_id_.fetch_add(1);
        tm_id_state_map_.emplace(s.tm_id, IdState::Running);
        s.tm_is_repeated = true;
        s.tm_repeated_num = repeated_num;
        s.tm_period = std::chrono::milliseconds(ms_time);
        s.tm_point = std::chrono::high_resolution_clock::now() + s.tm_period;
        s.callback = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lock(tm_mutex_);
        tm_queue_.push(s);
        tm_cond_.notify_all();
        return s.tm_id;
    }

    void cancel(int id) {
        if (tm_id_state_map_.isKeyExist(id)) {
            tm_id_state_map_.erase(id);
        }
    }

private:
    void runLocal() {
        while (tm_running_.load()) {
            std::unique_lock<std::mutex> lock(tm_mutex_);
            if (tm_queue_.empty()) {
                tm_cond_.wait(lock);
                continue;
            }
            auto s = tm_queue_.top();
            auto diff = s.tm_point - std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() > 0) {
                tm_cond_.wait_for(lock, diff);
                continue;
            }
            else {
                tm_queue_.pop();
                if (!tm_id_state_map_.isKeyExist(s.tm_id)) continue;
                if (s.tm_is_period) {
                    s.tm_point = std::chrono::high_resolution_clock::now() + s.tm_period;
                    tm_queue_.push(s);
                }
                else if (s.tm_is_repeated && s.tm_repeated_num > 0) {
                    s.tm_point = std::chrono::high_resolution_clock::now() + s.tm_period;
                    s.tm_repeated_num--;
                    tm_queue_.push(s);
                }
                lock.unlock();
                tm_thread_pool_.submit(std::move(s.callback));
            }
        }
    }

private:
    std::priority_queue<TimerNode> tm_queue_;
    std::atomic<bool> tm_running_;
    std::mutex tm_mutex_;
    std::condition_variable tm_cond_;
    std::thread tm_tick_thread_;
    ThreadPool tm_thread_pool_;
    std::atomic<int> tm_id_;
    HashMap<int, IdState> tm_id_state_map_;
};