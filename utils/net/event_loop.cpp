#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>

#include "event_loop.h"
#include "../log/pr.h"
#include "../log/log.h"

EventLoop::EventLoop(): el_epoller_(new Epoll()) {
    if (el_evfd_ = {eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)}; el_evfd_ < 0) {
        PR_ERROR("fail to create event_fd\n");
        exit(1);
    }
    LOG_INFO("create one eventloop, event fd is %d\n", el_evfd_);
    el_epoller_->epollAdd(el_evfd_, EPOLLIN, [this](){ this->evfdRead();});
}

EventLoop::~EventLoop() {
    close(el_evfd_);
}

void EventLoop::evfdWakeup() {
    uint64_t one = 1;
    if(auto n = write(el_evfd_, &one, sizeof one); n != sizeof one)
    {
        PR_ERROR("write %ld bytes to event_fd instead of 8\n", n);
    }
}

void EventLoop::evfdRead() {
    uint64_t one = 1;
    if (auto n = read(el_evfd_, &one, sizeof one); n != sizeof one) {
        PR_ERROR("read %ld bytes from event_fd instead of 8\n", n);
    }
}

void EventLoop::addTask(Task&& cb) {
    LOG_INFO("eventloop, add one task\n");
    if (isInLoopThread()) {
        cb();
    }
    else {
        std::lock_guard<std::mutex> lock(el_mutex_);
        el_task_funcs_.emplace_back(move(cb));
    }
    if (!isInLoopThread() || el_dealing_task_funcs) {
        evfdWakeup();
    }
}

void EventLoop::loop() {
    el_quit_ = false;
    while(!el_quit_) {
        auto cnt = el_epoller_->poll();
        LOG_INFO("eventloop, tid %lld, loop once, epoll event cnt %d\n", tidToLl(std::this_thread::get_id()), cnt);
        executeTaskFuncs();
    }
}

void EventLoop::executeTaskFuncs() {
    std::vector<Task> functions;
    el_dealing_task_funcs = true;
    {
        std::lock_guard<std::mutex> lock(el_mutex_);
        functions.swap(el_task_funcs_);
    }
    for (size_t i=0; i<functions.size(); ++i) functions[i]();
    el_dealing_task_funcs = false;
}

void EventLoop::quit() {
    el_quit_ = true;
    if (!isInLoopThread()) {
        evfdWakeup();
    }
}