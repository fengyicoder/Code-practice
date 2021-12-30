
#include <iostream>
#include <thread>

using namespace std;

struct func
{
    int& i;
    func(int&i_):i(i_) {}
    void operator()()
    {
        for(unsigned int j=0; j<1000000; ++j);
    }
};

class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(thread &t_): t(t_) {}
    ~thread_guard() {
        if (t.joinable()) t.join();
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_): t(std::move(t_)) {
        if(!t.joinable()) throw std::logic_error("No thread");
    }
    ~scoped_thread()
    {
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};


int main() {
    int some_local_state=0;
    func my_func(some_local_state);
    // std::thread t(my_func);
    // thread_guard g(t);
    scoped_thread t(std::thread(my_func));
    cout << "the thead end" << endl;
    return 0;
}