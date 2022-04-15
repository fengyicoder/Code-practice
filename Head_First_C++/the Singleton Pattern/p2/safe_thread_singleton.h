#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

class Singleton
{
protected:
    Singleton() {}

private:
    static std::mutex m_mutex;
    static std::atomic<Singleton *> m_instance;

public:
    static Singleton *getInstance()
    {
        Singleton *tmp = m_instance.load(std::memory_order_acquire);
        if (tmp == nullptr)
        {
            // std::scoped_lock(m_mutex);
            std::lock_guard<std::mutex> lock(m_mutex);
            tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp == nullptr)
            {
                tmp = new Singleton;
                m_instance.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(Singleton &&) = delete;
};

std::mutex Singleton::m_mutex;
std::atomic<Singleton *> Singleton::m_instance(nullptr);