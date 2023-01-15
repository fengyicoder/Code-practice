#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "epoll.h"
#include "../log/log.h"

const int KEventsNum = 4096;
const int KEpollWaitTime = 20000;

Epoll::Epoll(): ep_epoll_fd_(epoll_create1(EPOLL_CLOEXEC)), ep_events_(KEventsNum) {
    assert(ep_epoll_fd_ > 0);
}

Epoll::~Epoll() {}

void Epoll::epollAdd(int fd, int event, const EventCallback& cb) {
    int final_events;
    int op;

    if (auto ret = ep_event_map_.find(fd); ret == ep_event_map_.end()) {
        final_events = event;
        op = EPOLL_CTL_ADD;
    }
    else {
        final_events = ret->second.event | event;
        op = EPOLL_CTL_MOD;
    }

    if (event & EPOLLIN) {
        ep_event_map_[fd].read_callback = cb;
    }
    else if (event & EPOLLOUT) {
        ep_event_map_[fd].write_callback = cb;
    }

    ep_event_map_[fd].event = final_events;

    struct epoll_event ev;
    ev.events = final_events;
    ev.data.fd = fd;
    if (epoll_ctl(ep_epoll_fd_, op, fd, &ev) == -1) {
        PR_ERROR("epoll ctl error for fd %d\n", fd);
        return;
    }
    LOG_INFO("epoll add, fd is %d, event is %d\n", fd, final_events);
    ep_listen_fds_.insert(fd);
}

void Epoll::epollDel(int fd, int event) {
    ep_event_map_iter ret;
    if (ret = ep_event_map_.find(fd); ret == ep_event_map_.end()) {
        return;
    }

    int &target_event = ret->second.event;

    if (target_event = target_event & (~event); target_event == 0) {
        this->epollDel(fd);
    }
    else {
        struct epoll_event ev;
        ev.events = target_event;
        ev.data.fd = fd;
        if (epoll_ctl(ep_epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
            PR_ERROR("epoll ctl error for fd %d\n", fd);
            return;
        }  
    }
}

void Epoll::epollDel(int fd) {
    ep_event_map_.erase(fd);
    ep_listen_fds_.erase(fd);
    epoll_ctl(ep_epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
}

int Epoll::poll() {
    while (true) {
        int event_count = epoll_wait(ep_epoll_fd_, &*ep_events_.begin(), ep_events_.size(), KEpollWaitTime);
        if (event_count < 0) {
            PR_ERROR("epoll wait return val <0! error no:%d, error str:%s\n", errno, strerror(errno));
            continue;
        }
        executeCbs(event_count);
        return event_count;
    } 
}

inline void Epoll::executeCbs(int event_count) {
    for (int i=0; i<event_count; i++) {
        auto ev_ret = ep_event_map_.find(ep_events_[i].data.fd);
        assert(ev_ret != ep_event_map_.end());

        io_event *ev = &(ev_ret->second);

        if (ep_events_[i].events & EPOLLIN) {
            LOG_INFO("execute read cb\n");
            if (ev->read_callback) ev->read_callback();
        }
        else if (ep_events_[i].events & EPOLLOUT) {
            LOG_INFO("execute write cb\n");
            if (ev->write_callback) ev->write_callback();
        }
        else if (ep_events_[i].events & (EPOLLHUP|EPOLLERR)) {
            if (ev->read_callback) ev->read_callback();
            else if (ev->write_callback) ev->write_callback();
            else {
                LOG_INFO("get error, delete fd %d from epoll\n", ep_events_[i].data.fd);
                epoll_del(ep_events_[i].data.fd);
            }
        }
    }
}