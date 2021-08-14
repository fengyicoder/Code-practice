#include "duck.h"

using namespace std;

MallardDuck::MallardDuck() {
    flyBehavior = new FlyWithWings();
    quackBehavior = new Quack();
}

void MallardDuck::display() {
    cout << "I'm a real Mallard duck!" << endl;
}


int main() {
    Duck* mallard = new MallardDuck();
    mallard->performFly();
    mallard->performQuack();

    return 0;
}