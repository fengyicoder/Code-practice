#include "tcp_server.h"
#include "event_loop.h"
#include "pr.h"
#include "log.h"

class EchoServer
{
public:
    EchoServer(EventLoop* loop, const char *ip, uint16_t port) :
            es_loop_(loop), es_server_(loop, ip, port) 
    {
        es_server_.setConnectedCb([this](const TcpConnSP& conn){ this->echo_conneted_cb(conn); });
        es_server_.setMessageCb([this](const TcpConnSP& conn, InputBuffer* ibuf){ this->echo_message_cb(conn, ibuf); });
        es_server_.setCloseCb([this](){ this->echo_close_cb(); });
    };

    ~EchoServer() {};

    void start(int thread_num) { es_server_.setThreadNum(thread_num); es_server_.start(); }

    void set_tcp_cn_timeout_ms(int ms) { es_server_.setTcpConnTimeoutMs(ms); }

private:
    void echo_conneted_cb(const TcpConnSP& conn) {
        PR_INFO("one connected! peer addr is %s, local socket fd is %d\n", conn->getPeerAddr(), conn->getFd());
    }

    void echo_message_cb(const TcpConnSP& conn, InputBuffer* ibuf) {
        
        const char *msg = ibuf->getFromBuf();
        std::string msg_str(msg, msg+ibuf->length());
        ibuf->pop(ibuf->length());
        ibuf->adjust();
    
        PR_INFO("socket fd %d recv message:%s", conn->getFd(), msg_str.c_str());

        conn->send(msg_str.c_str(), msg_str.length());
    }

    void echo_close_cb() { PR_INFO("one connection closed in echo server!\n"); }

    TcpServer es_server_;
    EventLoop *es_loop_;
};


int main()
{   
    Logger::getInstance()->init(NULL);

    EventLoop base_loop;
    EchoServer server(&base_loop, "127.0.0.1", 8888);
    server.set_tcp_cn_timeout_ms(8000);
    server.start(2);
    base_loop.loop();
    
    return 0;
}