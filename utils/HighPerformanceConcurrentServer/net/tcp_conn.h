#pragma once

#include <memory>
#include <any>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>

#include "../memory/data_buf.h"

class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnSP;

class EventLoop;
class TcpServer;

class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::function<void(const TcpConnSP&)> ConnectionCallback;
    typedef std::function<void()> CloseCallback;
    typedef std::function<void(const TcpConnSP&, InputBuffer*)> MessageCallback;

    TcpConnection(TcpServer* server, EventLoop* loop, int sockfd, struct sockaddr_in& addr, socklen_t& len);
    ~TcpConnection();

    EventLoop* getLoop() const { return tc_loop_; }
    void addTask();
    void setContext(const std::any& context) { tc_context_ = context; }
    auto getContext() { return &tc_context_; }

    const char* getPeerAddr() { return inet_ntoa(tc_peer_addr_.sin_addr); }
    auto getFd() { return tc_fd_; }
    bool send(const char* data, int len);
    void setConnectedCb(const ConnectionCallback& cb) { tc_connected_cb_ = cb; }
    void setMessageCb(const MessageCallback& cb) { tc_message_cb_ = cb; }
    void setCloseCb(const CloseCallback& cb) { tc_close_cb_ = cb; }

    void connected();
    void activeClose() { doClose(); }
    void setTimerId(int id) { tc_timer_id = id; }
    int getTimerId() { return tc_timer_id; }

private:
    inline void setSockFd(int& fd);
    void doRead();
    void doWrite();
    void doClose();

private:
    TcpServer* tc_server_;
    EventLoop* tc_loop_;
    int tc_fd_;
    int tc_timer_id{-1};

    struct sockaddr_in tc_peer_addr_;
    socklen_t tc_peer_addrlen_;

    OutputBuffer tc_obuf_;
    InputBuffer tc_ibuf_;

    std::any tc_context_;

    ConnectionCallback tc_connected_cb_;
    MessageCallback tc_message_cb_;
    CloseCallback tc_close_cb_;
};