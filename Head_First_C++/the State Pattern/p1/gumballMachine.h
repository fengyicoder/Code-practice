#ifndef __GUMBALLMACHINE_H__
#define __GUMBALLMACHINE_H__


#include "concreteState.h"

class GumballMachine: public Gumball
{
public:
    GumballMachine(int numberGumballs) {
        // soldOutState = new SoldOutState(this);
        noQuarterState = new NoQuarterState(this);
        hasQuarterState = new HasQuarterState(this);
        soldState = new SoldState(this);
        this->count = numberGumballs;
        if (numberGumballs > 0) {
            state = noQuarterState;
        }
    }
    void insertQuarter() {
        state->insertQuarter();
    }
    void ejectQuarter() {
        state->ejectQuarter();
    }
    void turnCrank() {
        state->turnCrank();
        state->dispense();
    }
    void setState(State *state) {
        this->state = state;
    }
    void releaseBall() {
        cout << "A gumball comes rolling out the slot..." << endl;
        if (count != 0) {
            count = count - 1;
        }
    }
    State* getHasQuarterState() {
        return hasQuarterState;
    }
    State* getNoQuarterState() {
        return noQuarterState;
    }
    State* getSoldState() {
        return soldState;
    }
    // State* getSoldOutState() {
    //     return soldOutState;
    // }
    int getCount() {
        return count;
    }

public:
    // State *soldOutState;
    State *noQuarterState;
    State *hasQuarterState;
    State *soldState;
    State *state;
    int count = 0;
};

#endif