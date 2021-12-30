#ifndef __CAFFEINEBEVERAGE_H__
#define __CAFFEINEBEVERAGE_H__

#include <iostream>

using namespace std;

class CaffeineBeverage {
public:
    void prepareRecipe() {
        boilWater();
        brew();
        pourInCup();
        if (hook()) {
            addCondiments();
        }
    }

    virtual void brew() = 0;
    virtual void addCondiments() = 0;

    void boilWater() {
        cout << "Boiling water" << endl;
    }

    void pourInCup() {
        cout << "Pouring into cup" << endl;
    }

    bool hook() {
        return true;
    }

};

#endif