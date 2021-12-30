#ifndef __MENUITEM_H__
#define __MENUITEM_H__

#include <iostream>
#include <string>

using namespace std;

class MenuItem {
public:
    MenuItem(string name, string description, bool vegetarian, double price) {
        this->name = name;
        this->description = description;
        this->vegetarian = vegetarian;
        this->price = price;
    }
    string getName() {
        return name;
    }
    string getDescription() {
        return description;
    }
    double getPrice() {
        return price;
    }
    bool isVegetarian() {
        return vegetarian;
    }

public:
    string name;
    string description;
    bool vegetarian;
    double price;
};

#endif