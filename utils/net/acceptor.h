#pragma once

#include <functional>
#include <netinet/in.h>
#include <unistd.h>

class EventLoop;
class TcpServer;

class Acceptor {
public:
    Acceptor(TcpServer* server, EventLoop* loop, const char* ip, uint16_t port);
    ~Acceptor();

    bool isListenning() const { return ac_listening_; }
    void listen();

private:
    void doAccept();

private:
    TcpServer *ac_server_;
    int ac_listen_fd_;
    EventLoop *ac_loop_;
    bool ac_listening_;
    int ac_idle_fd_;
    sockaddr_in ac_server_addr_;
};