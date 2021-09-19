#include "duck.h"

using namespace std;

MallardDuck::MallardDuck() {
    flyBehavior = new FlyWithWings();
    quackBehavior = new Quack();
}

void MallardDuck::display() {
    cout << "I'm a real Mallard duck!" << endl;
}

ModelDuck::ModelDuck() {
    flyBehavior = new FlyNoWay();
    quackBehavior = new Quack();
}

void ModelDuck::display() {
    cout << "I'm a model duck!" << endl;
}

int main() {
    Duck* mallard = new MallardDuck();
    mallard->performFly();
    mallard->performQuack();

    Duck* model = new ModelDuck();
    model->performFly();
    model->setFlyBehavior(new FlyRocketPowered());
    model->performFly();

    return 0;
}