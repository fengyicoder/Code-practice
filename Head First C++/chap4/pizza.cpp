#include "pizza.h"

void Pizza::prepare() {
    cout << "Preparing" << name << endl;
    cout << "Tossing dough..." << endl;
    cout << "Adding sauce..." << endl;
    cout << "Adding toppings: " << endl;
    for (auto topping : toppings)
    {
        cout << topping << " ";
    }
}

void Pizza::bake() {
    cout << "Bake for 25 minutes at 350"<< endl;
}

void Pizza::cut() {
    cout << "Cutting the pizza into diagonal slices"<< endl;
}

void Pizza::box() {
    cout << "Place pizza in official PizzaStore box"<< endl;
}

string Pizza::getName() {
    return name;
}

NYStyleCheesePizza::NYStyleCheesePizza() 
{
    name = "NY Style Sauce and Cheese Pizza";
    dough = "Thin Crust Dough";
    sauce = "Marinara Sauce";
    toppings.push_back("Grated Reggiano Cheese");
}

void NYStyleCheesePizza::prepare() 
{
    Pizza::prepare();
}

void NYStyleCheesePizza::bake() 
{
    Pizza::bake();
}

void NYStyleCheesePizza::cut() 
{
    Pizza::cut();
}

void NYStyleCheesePizza::box() 
{
    Pizza::box();
}

string NYStyleCheesePizza::getName() {
    return Pizza::getName();
}

ChicagoStyleCheesePizza::ChicagoStyleCheesePizza() 
{
    name = "Chicago Style Deep Dish Cheese Pizza";
    dough = "Extra Thick Crust Dough";
    sauce = "Plum Tomato Sauce";
    toppings.push_back("Shredded Mozzarella Cheese");
}

void ChicagoStyleCheesePizza::prepare() 
{
    Pizza::prepare();
}

void ChicagoStyleCheesePizza::bake() 
{
    Pizza::bake();
}

void ChicagoStyleCheesePizza::cut() 
{
    cout << "Cutting the pizza into square slices" << endl;
}

void ChicagoStyleCheesePizza::box() 
{
    Pizza::box();
}

string ChicagoStyleCheesePizza::getName() {
    return Pizza::getName();
}

Pizza* PizzaStore::orderPizza()
{
    Pizza* pizza;
    pizza = createPizza();
    pizza->prepare();
    pizza->bake();
    pizza->cut();
    pizza->box();

    return pizza;
}

Pizza* NYStylePizzaStore::createPizza()
{
    return new NYStyleCheesePizza();
}

Pizza* ChicagoStylePizzaStore::createPizza()
{
    return new ChicagoStyleCheesePizza();
}

int main()
{
    PizzaStore *nyStore = new NYStylePizzaStore();
    PizzaStore *chicagoStore = new ChicagoStylePizzaStore();
    Pizza *pizza = nyStore->orderPizza();
    cout << "Ethan ordered a " << pizza->getName() << endl;
    pizza = chicagoStore->orderPizza();
    cout << "Ethan ordered a " << pizza->getName() << endl;

    return 0;
}
