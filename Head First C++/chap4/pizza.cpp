#include "pizza.h"

// void Pizza::prepare() {
//     cout << "Preparing" << name << endl;
//     cout << "Tossing dough..." << endl;
//     cout << "Adding sauce..." << endl;
//     cout << "Adding toppings: " << endl;
//     for (auto topping : toppings)
//     {
//         cout << topping << " ";
//     }
// }

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

Dough* NYPizzaIngredientFactory::createDough()
{
    return new NewYorkDough();
}

Sauce* NYPizzaIngredientFactory::createSauce()
{
    return new NewYorkSauce();
}

Cheese* NYPizzaIngredientFactory::createCheese()
{
    return new NewYorkCheese();
}

vector<Veggies*> NYPizzaIngredientFactory::createVeggies()
{
    vector<Veggies*> veggies;
    veggies.push_back(new NewYorkVeggies());
    return veggies;
}

Pepperoni* NYPizzaIngredientFactory::createPepperoni()
{
    return new NewYorkPepperoni();
}

Clams* NYPizzaIngredientFactory::createClams()
{
    return new NewYorkClams();
}

Dough* ChicagoPizzaIngredientFactory::createDough()
{
    return new ChicagoDough();
}

Sauce* ChicagoPizzaIngredientFactory::createSauce()
{
    return new ChicagoSauce();
}

Cheese* ChicagoPizzaIngredientFactory::createCheese()
{
    return new ChicagoCheese();
}

vector<Veggies*> ChicagoPizzaIngredientFactory::createVeggies()
{
    vector<Veggies*> veggies;
    veggies.push_back(new ChicagoVeggies());
    return veggies;
}

Pepperoni* ChicagoPizzaIngredientFactory::createPepperoni()
{
    return new ChicagoPepperoni();
}

Clams* ChicagoPizzaIngredientFactory::createClams()
{
    return new ChicagoClams();
}

NYStyleCheesePizza::NYStyleCheesePizza(PizzaIngredientFactory *ingredientFactory) 
{
    name = "NY Style Sauce and Cheese Pizza";
    this->ingredientFactory = ingredientFactory;
}

void NYStyleCheesePizza::prepare() 
{
    cout << "Preparing " << name << endl;
    dough = ingredientFactory->createDough();
    sauce = ingredientFactory->createSauce();
    cheese = ingredientFactory->createCheese();
    clam = ingredientFactory->createClams();
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

ChicagoStyleCheesePizza::ChicagoStyleCheesePizza(PizzaIngredientFactory *ingredientFactory) 
{
    name = "Chicago Style Deep Dish Cheese Pizza";
    this->ingredientFactory = ingredientFactory;
}

void ChicagoStyleCheesePizza::prepare() 
{
    cout << "Preparing " << name << endl;
    dough = ingredientFactory->createDough();
    sauce = ingredientFactory->createSauce();
    cheese = ingredientFactory->createCheese();
    clam = ingredientFactory->createClams();
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
    Pizza* pizza = nullptr;
    PizzaIngredientFactory *ingredientFactory = new NYPizzaIngredientFactory();
    pizza = new NYStyleCheesePizza(ingredientFactory);
    return pizza;
}

Pizza* ChicagoStylePizzaStore::createPizza()
{
    Pizza* pizza = nullptr;
    PizzaIngredientFactory *ingredientFactory = new ChicagoPizzaIngredientFactory();
    pizza = new ChicagoStyleCheesePizza(ingredientFactory);
    return pizza;
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
