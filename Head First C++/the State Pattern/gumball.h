#ifndef __GUMBALL_H__
#define __GUMBALL_H__

#include "state.h"

class Gumball {
public:
    Gumball() { }
    virtual ~Gumball() { }
    virtual void insertQuarter() { }
    virtual void ejectQuarter() { }
    virtual void turnCrank() { }
    virtual void setState(State *state) { }
    virtual void releaseBall() { }
    virtual State* getHasQuarterState() { return nullptr;}
    virtual State* getNoQuarterState() { return nullptr;}
    virtual State* getSoldState() { return nullptr;}
    // virtual State* getSoldOutState();
    virtual int getCount() { return 0;}
};

#endif