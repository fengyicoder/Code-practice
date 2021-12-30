
//从不同线程读写数据
#include <vector>
#include <atomic>
#include <iostream>

std::vector<int> data;
std::atomic<bool> data_ready(false);

void reader_thread()
{
    while (!data_ready.load())
    {
        std::this_thread::sleep(std::milliseconds(1));
    }
    std::cout << "The answer=" << data[0] << "\n";
}

void write_thread()
{
    data.push_back(42);
    data_ready = true;
}

//参数中求值顺序对函数调用是未指定的
#include <iostream>
void foo(int a, int b)
{
    std::cout << a << "," << b << std::endl;
}

int get_num()
{
    static int i = 0;
    return ++i;
}

int main()
{
    foo(get_num(), get_num()); //无序调用get_num
}

//序列一致意味着全序
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
    x.store(true, std::memory_order_seq_cst); //1
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst); //2
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst))
        ;
    if (y.load(std::memory_order_seq_cst)) //3
        ++z;
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
        ;
    if (x.load(std::memory_order_seq_cst)) //4
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(ready_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0); //5
}

//宽松操作只有很少的顺序要求
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed); //1
    y.store(true, std::memory_order_relaxed); //2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ;                                  //3
    if (x.load(std::memory_order_relaxed)) //4
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); //5
}

//多线程上的宽松操作
#include <thread>
#include <atomic>
#include <iostream>

std::atomic<int> x(0), y(0), z(0); //1
std::atomic<bool> go(false);       //2

unsigned const loop_count = 10;

struct read_values
{
    int x, y, z;
};

read_values values1[loop_count];
read_values values2[loop_count];
read_values values3[loop_count];
read_values values4[loop_count];
read_values values5[loop_count];

void increment(std::atomic<int> *var_to_inc, read_values *values)
{
    while (!go)
        std::this_thread::yield(); //3自旋，等待信号
    for (unsigned i = 0; i < loop_count; ++i)
    {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        var_to_inc->store(i + 1, std::memory_order_relaxed); // 4
        std::this_thread::yield();
    }
}

void read_vals(read_values *values)
{
    while (!go)
        std::this_thread::yield(); //5自旋，等待信号
    for (unsigned i = 0; i < loop_count; ++i)
    {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        std::this_thread::yield();
    }
}

void print(read_values *v)
{
    for (unsigned i = 0; i < loop_count; ++i)
    {
        if (i)
            std::cout << ",";
        std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << ")";
    }
    std::cout << std::endl;
}

int main()
{
    std::thread t1(increment, &x, values1);
    std::thread t2(increment, &y, values2);
    std::thread t3(increment, &z, values3);
    std::thread t4(read_values, values4);
    std::thread t5(read_values, values5);

    go = true; //6 开始执行主循环的信号

    t5.join();
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    print(values1); //7 打印结果
    print(values2);
    print(values3);
    print(values4);
    print(values5);
}

//获得释放并不意味着全序
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
    x.store(true, std::memory_order_release);
}

void write_y()
{
    y.store(true, std::memory_order_release);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire))
        ;
    if (y.load(std::memory_order_acquire)) //1
        ++z;
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire))
        ;
    if (x.load(std::memory_order_acquire)) //2
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(ready_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0); //3
}

//获得释放操作在宽松操作上强加顺序
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed); //1
    y.store(true, std::memory_order_release); //2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire))
        ;                                  //3
    if (x.load(std::memory_order_relaxed)) //4
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); //5
}

//使用获得和释放顺序传递同步
std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);

void thread_1()
{
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141, std::memory_order_relaxed);
    data[4].store(2003, std::memory_order_relaxed);
    sync1.store(true, std::memory_order_release); //1 设置sync1
}

void thread_2()
{
    while (!sync1.load(std::memory_order_acquire))
        ;                                         //2 循环直到sync1被设置
    sync2.store(true, std::memory_order_release); //3 设置sync2
}

void thread_3()
{
    while (!sync2.load(std::memory_order_acquire))
        ; //4 循环直到sync2被设置
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == 141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
}

//使用std::memory_order_consume同步数据
struct X
{
    int i;
    std::string s;
}

std::atomic<X *>
    p;
std::atomic<int> a;

void create_x()
{
    X *x = new X;
    x->i = 42;
    x->s = "hello";
    a.store(99, std::memory_order_relaxed); //1
    p.store(x, std::memory_order_release);  //2
}

void use_x()
{
    X *x;
    while (!(x = p.load(std::memory_order_consume))) //3
        std::this_thread::sleep(std::chrono::microseconds(1));
    assert(x->i == 42);                              //4
    assert(x->s == "hello");                         //5
    assert(a.load(std::memory_order_relaxed) == 99); //6
}

int main()
{
    std::thread t1(create_x);
    std::thread t2(use_x);
    t1.join();
    t2.join();
}

//宽松操作可以使用栅栏排序
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);            //1
    std::atomic_thread_fence(std::memory_order_release); //2
    y.store(true, std::memory_order_relaxed);            //3
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ;                                                //4
    std::atomic_thread_fence(std::memory_order_acquire); //5
    if (x.load(std::memory_order_relaxed))               //4
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); //7
}

//在非原子操作上强制顺序
#include <atomic>
#include <thread>
#include <assert.h>

bool x = false;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y()
{
    x = true; //1
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true, std::memory_order_relaxed); //2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ; //3
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x) //4
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); //5
}
