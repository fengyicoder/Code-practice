#ifndef __BEVERAGE_H__
#define __BEVERAGE_H__

#include <iostream>
#include <string>

using namespace std;

class Beverage {
public:
    virtual string getDescription();
    virtual double cost() = 0;

public:
    string description = "Unknown Beverage";
};

class CondimentDecorator: public Beverage {
    virtual string getDescription() = 0;
};

class Espresso: public Beverage {
public:
    Espresso();
    ~Espresso();

    double cost();
};

class HouseBlend: public Beverage {
public:
    HouseBlend();
    ~HouseBlend();

    double cost();
};

class Mocha: public CondimentDecorator {
public:
    Mocha(Beverage* beverage);
    ~Mocha();

    string getDescription();
    double cost();

public:
    Beverage *beverage;
};



#endif