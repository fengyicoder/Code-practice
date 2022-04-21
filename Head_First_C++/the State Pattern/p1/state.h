#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>
#include <string>

class State 
{
public:
    virtual void insertQuarter() = 0;
    virtual void ejectQuarter() = 0;
    virtual void turnCrank() = 0;
    virtual void dispense() = 0;
};

#endif