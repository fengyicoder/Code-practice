#include <list>
#include <mutex>
#include <algorithm>
#include <thread>
#include <iostream>
#include <unistd.h>

std::list<int> some_list;
std::mutex some_mutex;
int flag = 1;

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

void func(int value_to_find)
{
    std::cout << "start thead" << std::endl;
    while (flag)
    {
        int ret = list_contains(value_to_find);
        std::cout << ret << std::endl;
    }
}

int main()
{
    std::thread t(func, 3);
    for (int i=0; i < 5; i++)
    {
        std::cout << "the value is " << i << std::endl;
        add_to_list(i);
        usleep(500);
    } 
    flag = 0;
    t.join();

    return 0;
}
