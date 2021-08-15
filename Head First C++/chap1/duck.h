#ifndef __DUCK_H__
#define __DUCK_H__

#include <iostream>
using namespace std;

class FlyBehavior {
public:
    virtual void fly() = 0;
};

class QuackBehavior {
public:
    virtual void quack() = 0;
};

class FlyWithWings: public FlyBehavior {
public:
    void fly() {
        cout << "I'm flying!" << endl;
    }
};

class FlyNoWay: public FlyBehavior {
public:
    void fly() {
        cout << "I can't fly" << endl;
    }
};

class Quack: public QuackBehavior {
public: 
    void quack() {
        cout << "Quack" << endl;
    }
};

class MuteQuack: public QuackBehavior {
public: 
    void quack() {
        cout << "Slience" << endl;
    }
};

class Squeak: public QuackBehavior {
public: 
    void quack() {
        cout << "Squeak" << endl;
    }
};

class Duck {
public:
    Duck(){};
    ~Duck(){};
    void performFly() {
        flyBehavior->fly();
    }
    void performQuack() {
        quackBehavior->quack();
    }
    void swim() {
        cout << "All ducks float, even decoys" << endl;
    }

public:
    FlyBehavior *flyBehavior;
    QuackBehavior *quackBehavior;

};

class MallardDuck: public Duck {
public:
    MallardDuck();
    ~MallardDuck();
    void display();

};

#endif