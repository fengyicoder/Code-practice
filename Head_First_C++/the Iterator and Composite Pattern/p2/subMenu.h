#ifndef __SUBMENU_H__
#define __SUBMENU_H__

#include <iostream>
#include <string>
#include <vector>

#include "menu.h"

using namespace std;

class SubMenu: public Menu {
public:
    SubMenu(string n): name(n) {}
    ~SubMenu() {}
    void addMenuItem(Menu *m) {
        menuVector.push_back(m);
    }

    void print(int indent) {
        for (int i=0; i<indent; i++) {
            cout << "  ";
        }
        cout << name << "(Sub):" << endl;
        vector<Menu*>::const_iterator it = menuVector.begin();
        while (it != menuVector.end()) {
            (*it)->print(indent + 1);
            it++;
        }
    }
public:
    vector<Menu*> menuVector;
    string name;
};

#endif