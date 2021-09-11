#include "coffee.h"
#include "tea.h"

int main() {
    CaffeineBeverage *caffeineBeverage = new Coffee();
    caffeineBeverage->prepareRecipe();
    caffeineBeverage = new Tea();
    caffeineBeverage->prepareRecipe();

    return 0;
}