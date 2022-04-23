

template<class T>
class SmartPtr {
public:
    SmartPtr(T* realPtr=0);
    SmartPtr(SmartPtr& rhs);
    ~SmartPtr();
    SmartPtr& operator=(SmartPtr& rhs);
    T* operator->() const;
    T& operator*() const;
    operator void*() {
        return (void*)pointee;
    }

    bool operator!() {
        if (!pointee) return true;
        else return false;
    }
    //尽量不要提供
    operator T*() {
        return pointee;
    }

    template<class newType>
    operator SmartPtr<newType>() {
        return SmartPtr<newType>(pointee);
    }
private:
    T *pointee;
};

template<class T>
SmartPtr<T>::SmartPtr(SmartPtr& rhs) {
    pointee = rhs.pointee;
    rhs.pointee = nullptr;
}

template<class T>
SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr& rhs) {
    if (this == &rhs) return *this;
    delete pointee;
    pointee = rhs.pointee;
    rhs.pointee = nullptr;
    return *this;
}

template<class T>
SmartPtr<T>::~SmartPtr() {
    if (pointee) delete pointee;
}

template<class T>
T& SmartPtr<T::operator*() const {
    if (pointee) return *pointee;
}

template<class T>
T* SmartPtr<T::operator->() const {
    if (pointee) return pointee;
}

template<class T>
class SmartPtrToConst {
    ...
protected:
    union 
    {
        const T* constPointee;
        T* pointee;
    };
    
};

template<class T>
class SmartPtr: public SmartPtrToConst<T {
    ...
}
