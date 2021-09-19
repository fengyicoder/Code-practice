
#include "gumballMachine.h"

int main() {
    Gumball *gumball = new GumballMachine(5);
    gumball->insertQuarter();
    gumball->turnCrank();

    return 0;
}