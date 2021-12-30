#include "breakfastMenu.h"
#include "dinerMenu.h"

int main() {
    Menu *breakfast = new BreakfastMenu();
    Iterator *b = breakfast->createIterator();
    Menu *diner = new DinerMenu();
    Iterator *d = diner->createIterator();

    while (b->hasNext()) {
        cout << b->next()->getName() << endl;
    }
    while (d->hasNext()) {
        cout << d->next()->getName() << endl;
    }

    return 0;
}