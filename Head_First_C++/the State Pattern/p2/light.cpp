#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "stdint.h"

using namespace std;

struct State;

class LightSwitch
{
    State* state;
public:
    LightSwitch() 
    {
        state = new OffState();
    }
    void set_state(State* state)
    {
        this->state = state;
    }
    void on() {
        state->on(this);
    }
    void off() {
        state->off(this);
    }
};

struct State
{
    virtual void on(LightSwitch* ls)
    {
        cout << "Light is already on\n";
    }

    virtual void off(LightSwitch* ls)
    {
        cout << "Light is already off\n"
    }
};

struct OnState: State
{
    OnState() {
        cout << "Light turned on\n";
        void off(LightSwitch* ls) override
        {
            cout << "Switching light off...\n";
            ls->set_state(new OffState());
            delete this;
        }
    }
};

struct OffState: State
{
    OffState() {
        cout << "Light turned off\n";
        void on(LightSwitch* ls) override
        {
            cout << "Switching light on...\n";
            ls->set_state(new OnState());
            delete this;
        }
    }
};

int main() {
    LightSwitch ls;
    ls.on();
    ls.off();
    ls.off();
    getchar();
    return 0;
}