#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

class Mediator
{
public:
    void send(string message, Colleague colleague) = 0;
};

class Colleague
{
protected:
    Mediator mediator;
public:
    Colleague(Mediator mediator)
    {
        this->mediator = mediator;
    }
};

class ConcreteColleague1: Colleague
{
public:
    ConcreteColleague1(Mediator mediator): mediator(mediator) {}
    void send(string message)
    {
        mediator.send(message, this);
    }

    void notify(string message)
    {
        cout << "同事1得到消息: " << message << endl;
    }
};

class ConcreteColleague2: Colleague
{
public:
    ConcreteColleague1(Mediator mediator): mediator(mediator) {}
    void send(string message)
    {
        mediator.send(message, this);
    }

    void notify(string message)
    {
        cout << "同事2得到消息: " << message << endl;
    }
};

class ConcreteMediator: Mediator
{
private:
    ConcreteColleague1 colleague1;
    ConcreteColleague2 colleague2;

public:
    void setColleague1(Colleague value)
    {
        colleague1 = value;
    }

    void setColleague2(Colleague value)
    {
        colleague2 = value;
    }

    void send(string message, Colleague colleague)
    {
        if (colleague == colleague1)
        {
            colleague2.notify(message);
        }
        else 
        {
            colleague1.notify(message);
        }
    }
};


int main()
{
    ConcreteMediator m = new ConcreteMediator();
    ConcreteColleague1 c1 = new ConcreteColleague1(m);
    ConcreteColleague2 c2 = new ConcreteColleague2(m);

    m.setColleague1(c1);
    m.setColleague2(c2);

    c1.send("吃过饭了吗？");
    c2.send("没有呢，你打算请客？");
}
