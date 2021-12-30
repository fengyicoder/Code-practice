#ifndef __PIZZA_H__
#define __PIZZA_H__

#include <iostream>
#include <String>
#include <Vector>

using namespace std;

class Dough
{
public:
    Dough(){}
    ~Dough(){}
};

class Sauce
{
public:
    Sauce(){}
    ~Sauce(){}
};

class Cheese
{
public:
    Cheese(){}
    ~Cheese(){}
};

class Veggies
{
public:
    Veggies(){}
    ~Veggies(){}
};

class Pepperoni
{
public:
    Pepperoni(){}
    ~Pepperoni(){}
};

class Clams
{
public:
    Clams(){}
    ~Clams(){}
};

class Pizza {
public:
    virtual void prepare() = 0;
    virtual void bake() = 0;
    virtual void cut() = 0;
    virtual void box() = 0;
    virtual string getName() = 0;
public:
    string name;
    Dough* dough;
    Sauce* sauce;
    vector<Veggies*> veggies;
    Cheese* cheese;
    Pepperoni* pepperoni;
    Clams* clam;
};

class PizzaIngredientFactory {
public:
    virtual Dough* createDough() = 0;
    virtual Sauce* createSauce() = 0;
    virtual Cheese* createCheese() = 0;
    virtual vector<Veggies*> createVeggies() = 0;
    virtual Pepperoni* createPepperoni() = 0;
    virtual Clams* createClams() = 0;
};

class NYStyleCheesePizza: public Pizza {
public:
    NYStyleCheesePizza(PizzaIngredientFactory *ingredientFactory);
    ~NYStyleCheesePizza();
    void prepare();
    void bake();
    void cut();
    void box();
    string getName();
public:
    PizzaIngredientFactory *ingredientFactory;
};

class ChicagoStyleCheesePizza: public Pizza {
public:
    ChicagoStyleCheesePizza(PizzaIngredientFactory *ingredientFactory);
    ~ChicagoStyleCheesePizza();
    void prepare();
    void bake();
    void cut();
    void box();
    string getName();
public:
    PizzaIngredientFactory *ingredientFactory;
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


class NewYorkDough: public Dough
{
public:
    NewYorkDough(){}
    ~NewYorkDough(){}
};

class NewYorkSauce: public Sauce
{
public:
    NewYorkSauce(){}
    ~NewYorkSauce(){}
};

class NewYorkCheese: public Cheese
{
public:
    NewYorkCheese(){}
    ~NewYorkCheese(){}
};

class NewYorkVeggies: public Veggies
{
public:
    NewYorkVeggies(){}
    ~NewYorkVeggies(){}
};

class NewYorkPepperoni: public Pepperoni
{
public:
    NewYorkPepperoni(){}
    ~NewYorkPepperoni(){}
};

class NewYorkClams: public Clams
{
public:
    NewYorkClams(){}
    ~NewYorkClams(){}
};

class ChicagoDough: public Dough
{
public:
    ChicagoDough(){}
    ~ChicagoDough(){}
};

class ChicagoSauce: public Sauce
{
public:
    ChicagoSauce(){}
    ~ChicagoSauce(){}
};

class ChicagoCheese: public Cheese
{
public:
    ChicagoCheese(){}
    ~ChicagoCheese(){}
};

class ChicagoVeggies: public Veggies
{
public:
    ChicagoVeggies(){}
    ~ChicagoVeggies(){}
};

class ChicagoPepperoni: public Pepperoni
{
public:
    ChicagoPepperoni(){}
    ~ChicagoPepperoni(){}
};

class ChicagoClams: public Clams
{
public:
    ChicagoClams(){}
    ~ChicagoClams(){}
};

class NYPizzaIngredientFactory: public PizzaIngredientFactory {
public:
    Dough* createDough();
    Sauce* createSauce();
    Cheese* createCheese();
    vector<Veggies*> createVeggies();
    Pepperoni* createPepperoni();
    Clams* createClams();
};

class ChicagoPizzaIngredientFactory: public PizzaIngredientFactory {
public:
    Dough* createDough();
    Sauce* createSauce();
    Cheese* createCheese();
    vector<Veggies*> createVeggies();
    Pepperoni* createPepperoni();
    Clams* createClams();
};

#endif