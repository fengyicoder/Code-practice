#include <iostream>
#include "string.h"

using namespace std;

class String  {
public:
    String(const char* initValue=""): value(new StringValue(initValue))
    { }

    String(const String& rhs): value(rhs.value)
    {
        if (rhs.value->shareable)
        {
            value = rhs.value;
            ++value->refCount;
        }
        else{
            value = new StringValue(rhs.value->data);
        }
        
    }

    ~String() {
        if (--value->refCount == 0) delete value;
    }

    String& operator=(const String& rhs) {
        if (value == rhs.value)
        {
            return *this;
        }
        if (--value->refCount == 0) delete value;
        if (rhs.value->shareable)
        {
            value = rhs.value;
            ++value->refCount;
        }
        else {
            value = new StringValue(rhs.value->data);
        }
        return *this;
    }

    const char& operator[](int index) const {
        return value->data[index];
    }

    char& operator[](int index) {
        if (value->refCount > 1) {
            --value->refCount;
            value = new StringValue(value->data);
        }
        value->shareable = false;
        return value->data[index];
    }
private:
    struct StringValue {
        int refCount;
        bool shareable;
        char *data;
        StringValue(const char *initValue): refCount(1), shareable(true)
        {
            data = new char[strlen(initValue)+1];
            strcpy(data, initValue);
        }
        ~StringValue() {
            delete [] data;
        }
    };

    StringValue *value;
};

//引用计数成为基类的实现
class RCObject
{
public:
    RCObject(): refCount(0), shareable(true) {}
    RCObject(const RCObject& rhs): refCount(0), shareable(true) {}
    RCObject& operator=(const RCObject& rhs) {
        return *this;
    }
    virtual ~RCObject() {};
    void addReference() {
        ++refCount;
    }
    void removeReference() {
        if (--refCount == 0) delete this;
    }
    void markUnshareable() {
        shareable = false;
    }
    bool isShareable() const {
        return shareable;
    }
    bool isShared() const {
        return refCount > 1;
    }
private:
    int refCount;
    bool shareable;
};

template<class T>
class RCPtr {
public:
    RCPtr(T* realPtr = 0): pointee(realPtr) {
        init();
    }
    RCPtr(const RCPtr& rhs): pointee(rhs.pointee) {
        init();
    }
    ~RCPtr() {
        if(pointee) pointee->removeReference();
    }

    RCPtr& operator=(const RCPtr& rhs) {
        if (pointee != rhs.pointee) {
            if (pointee) pointee->removeReference();
            pointee = rhs.pointee;
            init();
        }
        return *this;
    }
    T* operator->() const {
        return pointee;
    }
    T& operator*() const {
        return *pointee;
    }
private:
    T *pointee;
    void init() {
        if (pointee == 0) return;
        if (pointee->isShareable() == false) {
            pointee = new T(*pointee);
        }
        pointee->addReference();
    }
};

class String
{
private:
    struct StringValue: public RCObject {
        char* data;
        StringValue(const char *initValue) {
            init(initValue);
        }
        StringValue(const StringValue& rhs) {
            init(rhs.data);
        }
        ~StringValue() {
            delete [] data;
        }
        void init(const char*initValue) {
            data = new char[strlen(initValue) + 1];
            strcpy(data, initValue);
        }
    };
    RCPtr<StingValue> value;
public:
    String(cosnt char *initValue): value(new StringValue(initValue)) {}
    const char& operator[](int index) const {
        return value->data[index];
    }
    char& operator[](int index) {
        if (value->isShared()) {
            value = new StringValue(value->data);
        }
        value->markUnshareable();
        return value->data[index];
    }
};

//将引用计数加到任意类型
template<class T>
class RCIPtr {
public:
    RCIPtr(T* realPtr = 0): counter(new CountHolder) {
        counter->pointee = realPtr;
        init();
    }
    RCIPtr(const RCIPtr& rhs): counter(rhs.counter) {
        init();
    }
    ~RCIPtr() {
        counter->removeReference();
    }

    RCIPtr& operator=(const RCIPtr& rhs) {
        if (counter != rhs.counter) {
            counter->removeReference();
            counter = rhs.counter;
            init();
        }
        return *this;
    }
    const T* operator->() const {
        return counter->pointee;
    }
    T* operator->() {
        makeCopy();
        return counter->pointee;
    }
    const T& operator*() const {
        return *(counter->pointee);
    }
    T& operator*() {
        makeCopy();
        return *(counter->pointee);
    }
private:
    struct CountHolder: public RCObject {
        ~CountHolder() {delete pointee;}
        T *pointee;
    };
    CountHolder *counter;
    void init() {
        if (counter->isShareable() == false) {
            T *oldValue = counter->pointee;
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
        }
        counter->addReference();
    }
    void makeCopy() {
        if (counter->isShareable()) {
            T *oldValue = counter->pointee;
            counter->removeReference();
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
            counter->addReference();
        }
    }
};

class Widget
{
public:
    Widget(int size);
    Widget(const Widget& rhs);
    ~Widget();
    Widget& operator=(const Widget& rhs);
    void doThis();
    int showThat() const;
};

class RCWidget {
public:
    RCWidget(int size): value(new Widget(size)) {}
    void doThis() { value->doThis();}
    int showThat() const {
        return value->showThat();
    }
private:
    RCIPtr<Widget> value;
};

int main() {
    return 0;
}