#include "beverage.h"

string Beverage::getDescription() {
    return description;
}

Espresso::Espresso() {
    description = "Espresso";
}

double Espresso::cost() {
    return 1.99;
}

HouseBlend::HouseBlend() {
    description = "HouseBlend";
}

double HouseBlend::cost() {
    return 0.89;
}

Mocha::Mocha(Beverage* beverage) {
    this->beverage = beverage;
}

string Mocha::getDescription() {
    return beverage->getDescription() + ", Mocha";
}

double Mocha::cost() {
    return 0.20 + beverage->cost();
}

int main() {
    Beverage *beverage = new Espresso();
    cout << beverage->getDescription() << " $" << beverage->cost() << endl;
    Beverage *beverage2 = new HouseBlend();
    cout << beverage2->getDescription() << " $" << beverage2->cost() << endl;
    beverage2 = new Mocha(beverage2);
    cout << beverage2->getDescription() << " $" << beverage2->cost() << endl;


}


