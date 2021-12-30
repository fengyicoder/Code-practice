#include "menuItem.h"
#include "subMenu.h"

int main() {
    Menu *menu = new SubMenu("Diner");
    menu->addMenuItem(new MenuItem("Meat", 12.0));
    Menu *desert = new SubMenu("Desert");
    menu->addMenuItem(desert);
    menu->print();

    return 0;
}