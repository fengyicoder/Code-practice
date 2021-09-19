#ifndef __CONCRETESTATE_H__
#define __CONCRETESTATE_H__


#include "state.h"
#include "gumball.h"

using namespace std;

class NoQuarterState: public State
{
public:
    NoQuarterState(Gumball *gumball) {
        this->gumball = gumball;
    }
    void insertQuarter() {
        cout << "You inserted a quarter" << endl;
        gumball->setState(gumball->getHasQuarterState());
    }
    void ejectQuarter() {
        cout << "You haven't inserted a quarter" << endl;
    }
    void turnCrank() {
        cout << "You turned, but there's no quarter" << endl;
    }
    void dispense() {
        cout << "You need to pay first" << endl;
    }

public:
    Gumball *gumball;
};

class HasQuarterState: public State
{
public:
    HasQuarterState(Gumball *gumball) {
        this->gumball = gumball;
    }
    void insertQuarter() {
        cout << "You can't inserted another quarter" << endl;
    }
    void ejectQuarter() {
        cout << "Quarter returned" << endl;
        gumball->setState(gumball->getNoQuarterState());
    }
    void turnCrank() {
        cout << "You turned..." << endl;
        gumball->setState(gumball->getSoldState());
    }
    void dispense() {
        cout << "No gumball dispensed" << endl;
    }

public:
    Gumball *gumball;
};

class SoldState: public State
{
public:
    SoldState(Gumball *gumball) {
        this->gumball = gumball;
    }
    void insertQuarter() {
        cout << "Please wait, we're already giving you a gumball!" << endl;
    }
    void ejectQuarter() {
        cout << "Sorry, you already turned the crank" << endl;
    }
    void turnCrank() {
        cout << "Turning twice doesn't get you another gumball!" << endl;
    }
    void dispense() {
        gumball->releaseBall();
        if (gumball->getCount() > 0) {
            gumball->setState(gumball->getNoQuarterState());
        }
        else {
            cout << "Oops, out of gumballs!" << endl;
            // gumball->setState(gumball->getSoldOutState());
        }
    }

public:
    Gumball *gumball;
};


#endif
