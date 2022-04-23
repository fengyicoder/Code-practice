
class PrintJob;

//第一种实现
class Printer {
public:
    void submitJob(const PrintJob& job);
    void reset();
    void performSelfTest();
    friend Printer& thePrinter();
private:
    Printer();
    Printer(const Printer& rhs);
};

Printer& thePrinter() {
    static Printer p;
    return p;
};

//第二种实现
class Printer {
public:
    static Printer& thePrinter();
private:
    Printer();
    Printer(const Printer& rhs);
}

Printer& Printer::thePrinter() {
    static Printer p;
    return p;
};

//带有计数器的实现
class Printer {
public:
    class TooManyObjects {}
    Printer();
    ~Printer();
private:
    static size_t numObjects;
    Printer(const Printer& rhs);
};

size_t Printer::numObjects = 0;
Printer::Printer()
{
    if (numObjects >= 1) {
        throw TooManyObjects();
    }
    ++numObjects;
}

Printer::~Printer() {
    --numObjects;
}

//计数器为基类
template<class BegingCounted>
class Counted {
public:
    class TooManyObjects{};
    static int objectCount() { return numObjects;}
protected:
    Counted();
    Counted(const Counted& rhs);
    ~Counted() { --numObjects;}
private:
    static int numObjects;
    static const size_t maxObjects;
    void init();
};

template<class BeingCounted>
Counted<BeingCounted>::Counted() {
    init();
}

template<class BeingCounted>
Counted<BeingCounted>::Counted(const Counted<BegingCounted>&) {
    init();
}

template<class BeingCounted>
Counted<BeingCounted>::init() {
    if (numObjects >= maxObjects) throw TooManyObjects();
    ++numObjects;
}

template<class BeingCounted>
Counted<BeingCounted>::numObjects;

template<class BeingCounted>
Counted<BeingCounted>::maxObjects = 16;

class Printer: private Counted<Printer> {
public:
    static Printer* makePrinter();
    static Printer* makePrinter(const Printer& rhs);
    ~Printer();
    void submitJob(const PrintJob& job);
    void reset();
    void performSelfTest();
    using Counted<Printer>::objectCount;
    using Counted<Printer>::TooManyObjects;
private:
    Printer();
    Printer(const Printer& rhs);
};
