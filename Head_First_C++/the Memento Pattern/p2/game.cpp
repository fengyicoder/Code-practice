#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "stdint.h"

using namespace std;

class Memento
{
private:
    string state;
public:
    Memento(string state)
    {
        this->state = state;
    }

    string get_state() const {
        return state;
    } 

    void set_state(string state) {
        this->state = state;
    }

    __declspec(property(get=get_state, put=set_state)) string State;
};

class Originator
{
private:
    string state;
public:

    Memento createMemento()
    {
        return (new Memento(state));
    }

    void setMemento(Memento memento)
    {
        state = memento.state;
    }

    void show()
    {
        cout << "State=" << state << endl;
    }

    string get_state() const {
        return state;
    } 

    void set_state(string state) {
        this->state = state;
    }

    __declspec(property(get=get_state, put=set_state)) string state;
};

class Caretaker
{
private:
    Memento memento;
public:
    Memento get_state() const {
        return memento;
    } 

    void set_state(Memento memento) {
        this->memento = memento;
    }

    __declspec(property(get=get_state, put=set_state)) Memento memento;
};

int main() {
    Originator o = new Originator();
    o.state = "On";
    o.show();
    
    Caretaker c = new Caretaker();
    c.memento = o.createMemento();

    o.state = "Off";
    o.show();

    o.setMemento(c.memento);
    o.show();

    return 0;
}