
#include <iostream>
#include <thread>
#include <vector>

void do_work(unsigned id) { std::cout << id << ": I'm working!" << std::endl; };

void f()
{
    std::vector<std::thread> threads;
    for (unsigned i=0; i<20; ++i)
    {
        threads.emplace_back(do_work, i);
    }
    for (auto& entry : threads) entry.join();

}

int main()
{
    f();
    std::cout << "the function end" << std::endl;

    return 0;
}