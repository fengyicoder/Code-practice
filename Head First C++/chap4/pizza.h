#ifndef __PIZZA_H__
#define __PIZZA_H__

#include <iostream>
#include <String>
#include <Vector>

using namespace std;

class Pizza {
public:
    virtual void prepare() = 0;
    virtual void bake() = 0;
    virtual void cut() = 0;
    virtual void box() = 0;
    virtual string getName() = 0;
public:
    string name;
    string dough;
    string sauce;
    vector<string> toppings;
};

class NYStyleCheesePizza: public Pizza {
public:
    NYStyleCheesePizza();
    ~NYStyleCheesePizza();
    void prepare();
    void bake();
    void cut();
    void box();
    string getName();
};

class ChicagoStyleCheesePizza: public Pizza {
public:
    ChicagoStyleCheesePizza();
    ~ChicagoStyleCheesePizza();
    void prepare();
    void bake();
    void cut();
    void box();
    string getName();
};

class PizzaStore {
public:
    Pizza* orderPizza();

protected:
    virtual Pizza* createPizza() = 0;
};

class NYStylePizzaStore: public PizzaStore{
public:
    Pizza* createPizza();

};

class ChicagoStylePizzaStore: public PizzaStore{
public:
    Pizza* createPizza();
};

#endif