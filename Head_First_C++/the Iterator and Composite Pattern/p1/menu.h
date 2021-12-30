#ifndef __MENU_H__
#define __MENU_H__

#include "iterator.h"


using namespace std;

class Menu {
public:
    virtual Iterator* createIterator() = 0;
};

#endif