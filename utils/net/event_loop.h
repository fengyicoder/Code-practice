#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <sys/eventfd.h>

#include "epoll.h"

class EventLoop {
public:
    typedef std::function<void()> Task;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void addTask(Task&& cb);
    void addToPoller(int fd, int event, const Epoll::EventCallback& cb) {
        el_epoller_->epollAdd(fd, event, cb);
    }

    void delFromPoller(int fd, int event) {
        el_epoller_->epollDel(fd, event);
    }

    void delFromPoller(int fd) {
        el_epoller_->epollDel(fd);
    }

    bool isInLoopThread() const {
        return el_tid_ == std::this_thread::get_id();
    }

private:
    void evfdWakeup();
    void evfdRead();
    void executeTaskFuncs();

private:
    std::shared_ptr<Epoll> el_epoller_;
    bool el_quit_{false};

    const std::thread::id el_tid_{std::this_thread::get_id()};
    std::mutex el_mutex_;

    int el_evfd_;
    std::vector<Task> el_task_funcs_;
    bool el_dealing_task_funcs{false};
};