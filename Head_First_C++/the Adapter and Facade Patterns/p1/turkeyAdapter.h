#ifndef __TURKEYADAPTER_H__
#define __TURKEYADAPTER_H__

#include "duck.h"
#include "turkey.h"

class TurkeyAdapter: public Duck {
public:
    TurkeyAdapter(Turkey *turkey)
    {
        this->turkey = turkey;
    }
    void quack() {
        turkey->gobble();
    }
    void fly() {
        for (int i=0; i<5; i++) {
            turkey->fly();
        }
    }

public:
    Turkey* turkey;
};

#endif