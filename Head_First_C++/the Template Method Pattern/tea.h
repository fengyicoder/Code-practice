#ifndef __TEA_H__
#define __TEA_H__

#include "caffeineBeverage.h"

class Tea: public CaffeineBeverage {
public:
    void brew() {
        cout << "Steeping the tea" << endl;
    }

    void addCondiments() {
        cout << "Adding Lemon" << endl;
    }
};

#endif