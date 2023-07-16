#pragma once

#include <sys/epoll.h>
#include <functional>
#include <unordered_map>
#include <set>
#include <vector>

class Epoll {
public:
    typedef std::function<void()> EventCallback;

    struct io_event
    {
        int event;
        EventCallback read_callback;
        EventCallback write_callback;
    };

    Epoll();
    ~Epoll();
    void epollAdd(int fd, int event, const EventCallback& cb);
    void epollDel(int fd, int event);
    void epollDel(int fd);
    int poll();
    int getEpollFd() {
        return ep_epoll_fd_;
    }
    void getListenFds(std::set<int>& fd_set) {
        fd_set = ep_listen_fds_;
    }

private:
    void executeCbs(int event_count);

private:
    static const int MAXFDS = 100000;
    int ep_epoll_fd_;
    std::set<int> ep_listen_fds_;
    
    typedef std::unordered_map<int, io_event>::iterator ep_event_map_iter;
    std::unordered_map<int, io_event> ep_event_map_;
    std::vector<epoll_event> ep_events_;
};