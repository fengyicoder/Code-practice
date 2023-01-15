#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "tcp_conn.h"
#include "tcp_server.h"
#include "event_loop.h"
#include "../log/pr.h"
#include "../log/log.h"

TcpConnection::TcpConnection(TcpServer* server, EventLoop* loop, int sockfd, struct sockaddr_in& addr, socklen_t& len) {
    tc_server_ = server;
    tc_peer_addr_ = addr;
    tc_peer_addrlen_ = len;
    tc_loop_ = loop;
    tc_fd_ = sockfd;

    setSockFd(tc_fd);
}

TcpConnection::~TcpConnection() {
    LOG_INFO("TcpConnection descontructed, fd is %d\n", tc_fd_);
}

void TcpConnection::setSockFd(int& fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK|flag);
    int op = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &op, sizeof(op));
}

void TcpConnection::doRead() {
    if (int ret = tc_ibuf_.readFromFd(tc_fd_); ret == -1) {
        PR_ERROR("read data from socket error\n");
        this->doClose();
        return;
    }
    else if (ret == 0) {
        LOG_INFO("connection closed by peer\n");
        this->doClose();
        return;
    }

    tc_message_cb_(shared_from_this(), &tc_ibuf_);
    return;
}

void TcpConnection::doWrite() {
    while(tc_obuf_.length()) {
        int ret;
        if (ret = tc_obuf_.write2fd(tc_fd_); ret == -1) {
            PR_ERROR("write2fd error, close conn!\n");
            this->doClose();
            return ;
        }
        if (ret == 0) {
            break;
        }
    }

    if (tc_obuf_.length() == 0) {
        tc_loop_->delFromPoller(tc_fd_, EPOLLOUT);
    }

    return;
}

void TcpConnetion::doClose() {
    if (tc_close_cb_) tc_close_cb_();
    tc_loop_->delFromPoller(tc_fd_);
    tc_ibuf_.clear();
    tc_obuf_.clear();

    int fd = tc_fd_;
    tc_fd_ = -1;
    close(fd);
    tc_server_->doClean(shared_from_this());
}

bool TcpConnection::send(const char*data, int len) {
    bool should_activate_epollout = false;
    if (tc_obuf_.length() == 0) {
        should_activate_epollout = true;
    }

    if (int ret = tc_obuf_.write2buf(data, len); ret != 0) {
        PR_ERROR("send data to output buf error\n");
        return false;
    }

    if (should_activate_epollout == true) {
        tc_loop_->addToPoller(tc_fd_, EPOLLOUT, [this](){ this->doWrite(); });
    }
    return true;
}

void TcpConnection::connected() {
    if (tc_connected_cb_) {
        LOG_INFO("execute connected callback, conn fd is %d\n", tc_fd_);
        tc_connected_cb_(shared_from_this());
    }
    else {
        LOG_INFO("tcp connected callback is null\n");
    }
}