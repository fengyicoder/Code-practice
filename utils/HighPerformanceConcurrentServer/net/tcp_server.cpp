#include <cstring>
#include <arpa/inet.h>
#include <signal.h>

#include "../log/pr.h"
#include "../log/log.h"
#include "../threadpool/thread_pool.h"
#include "acceptor.h"
#include "tcp_server.h"
#include "event_loop.h"

TcpServer::TcpServer(EventLoop* loop, const char *ip, uint16_t port) {
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
        PR_ERROR("ignore SIGHUP signal error\n");
    }
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        PR_ERROR("ignore SIGPIPE signal error\n");
    }

    ts_acceptor_loop_ = loop;
    this->ip_ = ip;
    this->port_ = port;
    ts_message_cb_ = [this](const TcpConnSP& conn, InputBuffer* ibuf) {
        updateConnTimeoutTime(conn);
        if (ts_msg_cb_) ts_msg_cb_(conn, ibuf);
    };
    ts_acceptor_ = std::make_unique<Acceptor>(this, loop, ip, port);
}

void TcpServer::start() {
    if (!ts_started_) {
        ts_timer_.run();
        ts_started_ = true;
        LOG_INFO("tcp server create thread pool, thread num is %d\n", ts_thread_num_);
        if (ts_thread_pool_ = std::make_unique<ThreadPool>(ts_thread_num_); ts_thread_pool_ == nullptr) {
            PR_ERROR("tcp_server failed to create thread_pool\n");
            exit(1);
        }
        for( int i=0; i<ts_thread_num_; i++) {
            ts_conn_loops_.emplace_back(new EventLoop());
            EventLoop *ev = ts_conn_loops_[i];
            LOG_INFO("tcp server add loop_task to thread pool\n");
            ts_thread_pool_->submit([&ev](){ ev->loop();});
        }
    }
}

EventLoop* TcpServer::getNextLoop() {
    int size;
    if (size == ts_conn_loops_.size(); size==0) { return nullptr; }
    ++ts_next_loop_;
    ts_next_loop_ = ts_next_loop_ % size;
    return ts_conn_loops_[ts_next_loop_];
}

void TcpServer::doClean(const TcpConnSP& tcp_conn) {
    std::lock_guard<std::mutex> lock(ts_mutex_);
    for (auto i=ts_tcp_connections_.begin(), e=ts_tcp_connections_.end(); i!=e; ++i) {
        if (tcp_conn == *i) {
            LOG_INFO("tcpserver do clean, erase tcp_conn\n");
            ts_tcp_connections_.erase(i);
            break;
        }
    }
}

TcpServer::~TcpServer() {
    
}