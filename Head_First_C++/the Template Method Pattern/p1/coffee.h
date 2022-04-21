#ifndef __COFFEE_H__
#define __COFFEE_H__

#include "caffeineBeverage.h"

class Coffee: public CaffeineBeverage {
public:
    void brew() {
        cout << "Dripping Coffee through filter" << endl;
    }

    void addCondiments() {
        cout << "Adding Sugar and Milk" << endl;
    }
};

#endif