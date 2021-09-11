#include "breakfastMenu.h"
#include "dinerMenu.h"

int main() {
    BreakfastMenu *breakfast = new BreakfastMenu();
    Iterator *b = breakfast->createIterator();
    DinerMenu *diner = new DinerMenu();
    Iterator *d = diner->createIterator();

    while (b->hasNext()) {
        cout << b->next()->getName() << endl;
    }
    while (d->hasNext()) {
        cout << d->next()->getName() << endl;
    }

    return 0;
}