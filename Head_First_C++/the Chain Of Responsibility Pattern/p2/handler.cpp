#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"
#include <boost/signals2.hpp>
using namespace boost::signals2;

using namespace std;

class Handler
{
protected:
    Handler successor;
public:
    void setSuccessor(Handler successor)
    {
        this->successor = successor;
    }

    void handleRequest(int request) = 0;
};

class ConcreteHandler1: Handler
{
public:
    void handleRequest(int request) override
    {
        if (request >= 0 && request < 10)
        {
            Console.WriteLine("{0} 处理请求 {1}",
                this.GetType().Name, request);
        }
        else if (successor != nullptr)
        {
            successor.handleRequest(request);
        }
    }
};

class ConcreteHandler2: Handler
{
public:
    void handleRequest(int request) override
    {
        if (request >= 10 && request < 20)
        {
            Console.WriteLine("{0} 处理请求 {1}",
                this.GetType().Name, request);
        }
        else if (successor != nullptr)
        {
            successor.handleRequest(request);
        }
    }
};

class ConcreteHandler3: Handler
{
public:
    void handleRequest(int request) override
    {
        if (request >= 20 && request < 30)
        {
            Console.WriteLine("{0} 处理请求 {1}",
                this.GetType().Name, request);
        }
        else if (successor != nullptr)
        {
            successor.handleRequest(request);
        }
    }
};

int main()
{
    Handler h1 = new ConcreteHandler1();
    Handler h2 = new ConcreteHandler2();
    Handler h3 = new ConcreteHandler3();
    h1.setSuccessor(h2);
    h2.setSuccessor(h3);

    int requests[8] = {2, 5, 14, 22, 18, 3, 27, 20};
    for_each(begin(requests), end(requests), h1.handleRequest);
    return 0;
}