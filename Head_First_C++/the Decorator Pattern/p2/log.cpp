#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "stdint.h"

using namespace std;

struct Logger
{
    function<void()> func;
    string name;
    Logger(const function<void()> &func, const string &name) : func{func}, name{name} {}
    void operator()() const
    {
        cout << "Entering" << name << endl;
        func();
        cout << "Exiting" << name << endl;
    }
};

//另一种实现
template <typename Func>
struct Logger2
{
    Func func;
    string name;
    Logger2(const Func &func, const string &name) : func{func}, name{name} {}
    void operator()() const
    {
        cout << "Entering" << name << endl;
        func();
        cout << "Exiting" << name << endl;
    }
};

template <typename Func>
auto make_logger2(Func func, const string &name)
{
    return Logger2<Func>{func, name};
}

template<typename R, typename... Args>
struct Logger3{
    function<R(Args...)> func;
    string name;

    Logger3(const function<R(Args...)>& func, const string& name):
        func{func}, name{name} {}
    
    R operator()(Args... args) const {
        cout << "Entering " << name << endl;
        R result = func(args...);
        cout << "Exiting " << name << endl;
        return R;
    }
};

template<typename R, typename... Args>
auto make_logger3(R (*func)(Args...), const string& name) {
    return Logger3<R(Args...)>{function<R(Args...)>(func), name};
}

int main()
{
    auto call = make_logger2([]()
                             { cout << "Hello!" << endl; },
                             "HelloFunction");
    call();
    return 0;
}