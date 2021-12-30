#ifndef __BREAKFASTMENU_H__
#define __BREAKFASTMENU_H__

#include "menu.h"
#include "menuItem.h"
#include "iterator.h"

class BreakfastMenu: public Menu {
public:
    BreakfastMenu() {
        addItem("Waffles", "Waffles, with your choice of blueberries or strawberries", true, 3.59);
    }

    void addItem(string name, string description, bool vegetarian, double price) {
        MenuItem *items = new MenuItem(name, description, vegetarian, price);
        menuItems.push_back(items);
    }

    Iterator* createIterator() {
        return new BreakfastMenuIterator(menuItems);
    }

public:
    vector<MenuItem*> menuItems;

};


#endif