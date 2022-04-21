#include <atomic>

// 非完全线程安全的。
// 引用计数
template<typename T>
class ReferenceCounter
{
    
    ReferenceCounter( ):count(0) {};
    ReferenceCounter(const ReferenceCounter&) = delete;
    ReferenceCounter& operator=(const ReferenceCounter&) = delete;
    ReferenceCounter(ReferenceCounter&&) = default
    ReferenceCounter& operator=(ReferenceCounter&&) = default;
    // 前置++, 这里不提供后置，因为后置返回一个ReferenceCounter的拷贝，而之前禁止ReferenceCounter拷贝。
    ReferenceCounter& operator++()
    {   
        count.fetch_add(1);
        return *this;
    }

    ReferenceCounter& operator--()
    {
        count.fetch_sub(1);
        return *this;
    }

    size_t getCount() const 
    {
        return count.load();
    }

    private:
        // 原子类型，或上锁
        std::atomic<size_t> count;
        
};

template <typename T>
class SharedPtr{
    
    explicit SharedPtr(T* ptr_ = nullptr) : ptr(ptr_) {
        count = new ReferenceCounter();
        if(ptr)
        {
            ++(*count);
        } 
    }
    ~SharedPtr() {
        --(*count);
        if(count->getCount() == 0)
        {
            delete ptr;
            delete count;
        }
    }

    SharedPtr(const SharedPtr<T>& other) : 
        ptr(other.ptr), 
        count(other.count)
    {
        ++(*count);
    }

    SharedPtr<T>& operator= (const SharedPtr<T>& other)
    {
        if(this != &other)
        {
            --(*count);
            if(count.getCount() == 0)
            {
                delete count;
                delete ptr;
            }
            ptr = other.ptr;
            count = other.count;
            ++(*count);
        }
        return *this;   
    }

    SharedPtr(SharedPtr&& other) = default;
    SharedPtr& operator=(SharedPtr&& other) = default;

    T& operator*() const { return *ptr; }
    T* operator->() const{
        return ptr;
    }
    T* get() const { return ptr; }
    
    int use_count() const { return count->getCount(); }

    bool unique() const { return use_count() == 1; }

    private:
        T* ptr;
        ReferenceCounter* count;
}