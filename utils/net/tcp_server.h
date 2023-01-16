#pragma once

#include <netinet/in.h>
#include <vector>
#include <chrono>
#include <mutex>

#include "tcp_conn.h"
#include "../timer/timer.h"
#include "../log/log.h"

class EventLoop;
class ThreadPool;
class Acceptor;

class TcpServer {
public:
    typedef TcpConnection::ConnectionCallback ConnectionCallback;
    typedef TcpConnection::CloseCallback CloseCallback;
    typedef TcpConnection::MessageCallback MessageCallback;

    friend class Acceptor;
    friend class TcpConnection;

    TcpServer(EventLoop* loop, const char *ip, uint16_t port);
    ~TcpServer();

    void setThreadNum(int t_num) { ts_thread_num_ = t_num; }
    EventLoop* getNextLoop();
    void start();
    void doClean(const TcpConnSP& tcp_conn);
    void setTcpConnTimeoutMs(int ms) { ts_tcp_conn_timeout_ms_ = ms; }
    void setConnectedCb(const ConnectionCallback& cb) { ts_connected_cb_ = cb; }
    void setMessageCb(const MessageCallback& cb) { ts_msg_cb_ = cb; }
    void setCloseCb(const CloseCallback& cb) { ts_close_cb_ = cb; }

private:
    void addNewTcpConn(const TcpConnSP& tcp_conn) {
        auto timer_id = ts_timer_.runAfter(ts_tcp_conn_timeout_ms_, false, [tcp_conn]{
            LOG_INFO("tcp conn timeout!\n");
            tcp_conn->activeClose();
        });
        tcp_conn->setTimerId(timer_id);
        ts_tcp_connections_.emplace_back(tcp_conn);
    }

    void updateConnTimeoutTime(const TcpConnSP& tcp_conn) {
        ts_timer_.cancel(tcp_conn->getTimerId());
        addNewTcpConn(tcp_conn);
    }

private:
    const char *ip_;
    uint16_t port_;
    std::unique_ptr<Acceptor> ts_acceptor_;
    EventLoop *ts_acceptor_loop_;
    std::vector<EventLoop*> ts_conn_loops_;
    std::unique_ptr<ThreadPool> ts_thread_pool_;
    int ts_thread_num_{ 1 };
    int ts_next_loop_{ -1 };
    Timer ts_timer_;
    int ts_tcp_conn_timeout_ms_{6000};
    std::mutex ts_mutex_;
    std::vector<TcpConnSP> ts_tcp_connections_;
    bool ts_started_;
    ConnectionCallback ts_connected_cb_;
    MessageCallback ts_msg_cb_;
    MessageCallback ts_message_cb_;
    CloseCallback ts_close_cb_;
};