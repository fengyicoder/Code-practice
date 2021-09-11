#ifndef __DINERMENU_H__
#define __DINERMENU_H__

#include "menuItem.h"
#include "iterator.h"

class DinerMenu {
public:
    DinerMenu() {
        addItem("Vegetarian BLT", "Bacon with lettuce & tomato on whole wheat", true, 2.99);
        addItem("Hotdog", "A hot dog, with saurkraut, relish, onions, topped with cheese", false, 3.05);
    }

    void addItem(string name, string description, bool vegetarian, double price) {
        MenuItem *item = new MenuItem(name, description, vegetarian, price);
        if (numberOfItems >= MAX_ITEMS) cout << "Sorry, menu is full" << endl;
        else {
            menuItems[numberOfItems] = item;
            numberOfItems++;
        }
    }

    Iterator* createIterator() {
        return new DinerMenuIterator(menuItems, 2);
    }

public:
    static const int MAX_ITEMS = 6;
    int numberOfItems = 0;
    MenuItem *menuItems[MAX_ITEMS];

};


#endif