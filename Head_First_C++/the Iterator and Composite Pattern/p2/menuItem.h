#ifndef __MENUITEM_H__
#define __MENUITEM_H__

#include <iostream>
#include <string>

#include "menu.h"

using namespace std;

class MenuItem: public Menu {
public:
    MenuItem(string n, double p): name(n), price(p) {}
    ~MenuItem(){}
    void print(int indent) {
        for (int i=0; i<indent; i++) {
            cout << "  ";
        }
        cout << name << ":" << price << endl;
    }
public:
    string name;
    double price;
};

#endif 