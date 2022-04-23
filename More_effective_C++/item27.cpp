
//仅在堆中建立对象
class UPNumber {
public:
    UPNumber();
    UPNumber(int initValue);
    UPNumber(double initValue);
    UPNumber(const UPNumber& rhs);
    void destroy() const { delete this;}
private:
    ~UPNumber();
}

//判断指针是否由operator new分配
class HeapTracked {
public: 
    class MissingAddress{};
    virtual ~HeapTracked() = 0;
    static void *operator new(size_t size);
    static void operator delete(void *ptr);
    bool isOnHeap() const;
private:
    typedef const void* RawAddress;
    static list<RawAddress> addresses;
};

list<RawAddress> HeapTracked::addresses;

HeapTracked::~HeapTracked() {}

void *HeapTracked::operator new(size_t size)
{
    void *memPtr = ::operator new(size);
    addresses.push_front(memPtr);
    return memPtr;
}

void HeapTracked::operator delete(void *ptr) {
    list<RawAddress>::iterator it = find(addresses.begin(), addresses.end(), ptr);
    if (it != addresses.end()) {
        addresses.erase(it);
        ::operator delete(ptr);
    }
    else {
        throw MissingAddress();
    }
}

bool HeapTracked::isOnHeap() const {
    const void *rawAddress = dynamic_cast<const void*>(this);
    list<RawAddress>::iterator it = find(addresses.begin(), addresses.end(), rawAddress);
    return it != addresses.end();
}

//禁止在堆上上生成对象
class UPNumber {
private:
    static void *operator new(size_t size);
    static void operator delete(void *ptr);
}