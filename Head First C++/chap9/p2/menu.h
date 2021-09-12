#ifndef __MENU_H__
#define __MENU_H__

class Menu {
public:
    class UnSupportedOperation{};
    Menu() {}
    virtual ~Menu(){}
    virtual void addMenuItem(Menu *menu)
    {
        throw UnSupportedOperation();
    }
    virtual void print(int indent = 0) = 0;
};

#endif