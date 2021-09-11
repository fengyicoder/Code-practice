#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <iostream>
#include <vector>
#include "menuItem.h"

using namespace std;

class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual MenuItem* next() = 0;
};

class DinerMenuIterator: public Iterator {
public:
    DinerMenuIterator(MenuItem *items[], int count) {
        this->items = items;
        this->count = count;
    }
    MenuItem* next() {
        MenuItem* menuItem = items[position];
        position++;
        return menuItem;
    }
    bool hasNext() {
        return position < count;
    }
public:
    MenuItem **items;
    int position = 0;
    int count = 0;
};

class BreakfastMenuIterator: public Iterator {
public:
    BreakfastMenuIterator(vector<MenuItem*> &items) {
        iterator = items.begin();
        end = items.end();
    }
    MenuItem* next() {
        MenuItem* menuItem = *(iterator);
        iterator++;
        return menuItem;
    }
    bool hasNext() {
        if (iterator != end) return true;
        else return false;
    }
public:
    vector<MenuItem*>::const_iterator iterator;
    vector<MenuItem*>::const_iterator end;
};

#endif