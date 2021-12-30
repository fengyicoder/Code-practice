#ifndef __DUCK_H__
#define __DUCK_H__

#include <iostream>
using namespace std;

class Duck {
public:
    virtual void quack() = 0;
    virtual void fly() = 0;
};


#endif