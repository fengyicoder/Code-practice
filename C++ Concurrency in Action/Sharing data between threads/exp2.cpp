#include <mutex>
#include <algorithm>
#include <thread>
#include <iostream>

class some_big_object;
void swap(some_big_object& lhs, some_big_object& rhs);

/* 使用普通方式
class X
{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd): some_detail(sd) {}
    // friend void swap(X& lhs, X&rhs)
    // {
    //     if (&lhs == &rhs) return;
    //     std::lock(lhs.m, rhs.m);
    //     std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    //     std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    //     swap(lhs.some_detail, rhs.some_detail);
    // }
    //使用scoped_lock改进
    friend void swap(X& lhs, X&rhs)
    {
        if (&lhs == &rhs) return;
        std::scoped_lock guard(lhs.m, rhs.m);
        swap(lhs.some_detail, rhs.some_detail);
    }
};
*/

//使用unique_lock改写
class X
{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd): some_detail(sd) {}
    friend void swap(X& lhs, X&rhs)
    {
        if (&lhs == &rhs) return;
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
        std::lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
};

class hierarchical_mutex
{
    std::mutex internal_mutex;

    unsigned long const hierarchy_value;
    unsigned long previout_hierarchy_value;

    static thread_local unsigned long this_thread_hierarchy_value;
    void check_for_hierarchy_violation()
    {
        if (this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value()
    {
        previout_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
public:
    explicit hierarchical_mutex(unsigned long value):
    hierarchy_value(value), previout_hierarchy_value(0) {}
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock()
    {
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated");
        this_thread_hierarchy_value = previout_hierarchy_value;
        internal_mutex.unlock();
    }

    bool try_lock()
    {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
};
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    return lk;
}
void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());
    do_something();
}

//合适粒度的上锁
void get_and_process_data()
{
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process = get_next_data_chunk();
    my_lock.unlock();
    result_type result=process(data_to_process);
    my_lock.lock();
    write_result(data_to_process,result);
}

int main()
{
    return 0;
}