#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>

#include "../log/pr.h"
#include "../log/log.h"
#include "event_loop.h"
#include "tcp_server.h"
#include "acceptor.h"

Acceptor::Acceptor(TcpServer* server, EventLoop* loop, const char* ip, uint16_t port):
    ac_server_(server), 
    ac_loop_(loop),
    ac_listening_(false),
    ac_idle_fd_(open("/dev/null", O_RDONLY | O_CLOEXEC)),
    ac_listen_fd_(socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP))
{
    LOG_INFO("create one acceptor, listen fd is %d\n", ac_listen_fd_);
    assert(ac_listen_fd_ >= 0);
    assert(ac_idle_fd_ >= 0);

    int op = 1;
    if (setsockopt(ac_listen_fd_, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)<0)) {
        PR_ERROR("set listen socket SO_REUSEADDR failed!\n");
    }

    memset(&ac_server_addr_, 0, sizeof(ac_server_addr_));
    ac_server_addr_.sin_family = AF_INET;
    inet_aton(ip, &ac_server_addr_.sin_addr);
    ac_server_addr_.sin_port = htons(port);

    LOG_INFO("acceptor bind, ip is %s, port is %d\n", ip, (int)port);
    if (::bind(ac_listen_fd_, (const struct sockaddr*)&ac_server_addr_, sizeof(ac_server_addr_)) < 0) {
        PR_ERROR("bind server port error!\n");
        exit(1);
    }
}

Acceptor::~Acceptor() {
    close(ac_listen_fd_);
    close(ac_idle_fd_);
}

void Acceptor::listen() {
    LOG_INFO("acceptor execute listen, listen fd is %d\n", ac_listen_fd_);
    if (::listen(ac_listen_fd_, 1000) == -1) {
        PR_ERROR("server listen error\n");
        exit(1);
    }
    ac_listening_ = true;
    ac_loop_->addToPoller(ac_listen_fd_, EPOLLIN, [this](){ this->doAccept();});
}

void Acceptor::doAccept() {
    int connfd;
    struct sockaddr_in conn_addr;
    socklen_t conn_addrlen = sizeof conn_addr;
    while (true)
    {
        if (connfd = accept(ac_listen_fd_, (struct sockaddr*)&conn_addr, &conn_addrlen); connfd == -1) {
            if (errno == EINTR) {
                PR_ERROR("accept fail, errno=EINTR, continue\n");
                continue;
            }
            else if (errno == EMFILE) {
                PR_WARN("accept fail, errno=EMFILE, use idle fd\n");
                close(ac_idle_fd_);
                ac_idle_fd_ = accept(ac_listen_fd_, NULL, NULL);
                close(ac_idle_fd_);
                ac_idle_fd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
            }
            else if (errno == EAGAIN) {
                PR_ERROR("accept fail, errno=EAGAIN, break\n");
                break;
            }
            else {
                PR_ERROR("accept fail, error no:%d, error str:%s\n", errno,strerror(errno));
                continue;
            }
        }
        else {
            LOG_INFO("accepted one connection, sock fd is %d\n", connfd);
            EventLoop *sub_loop = ac_server_->getNextLoop();
            TcpConnSP conn = std::make_shared<TcpConnection>(ac_server_, sub_loop, connfd, conn_addr, conn_addrlen);
            conn->setConnectedCb(ac_server_->ts_connected_cb_);
            conn->setMessageCb(ac_server_->ts_message_cb_);
            conn->setCloseCb(ac_server_->ts_close_cb_);
            conn->addTask();
            std::lock_guard<std::mutex> lock(ac_server_->ts_mutex_);
            ac_server_->addNewTcpConn(conn);
        }
    } 
}