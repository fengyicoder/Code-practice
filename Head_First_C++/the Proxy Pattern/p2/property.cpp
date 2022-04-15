#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

template <typename T>
struct Property
{
    T value;
    Property(const T initial_value) { *this = initial_value; }
    operator T()
    {
        return value;
    }

    T operator=(T new_value)
    {
        return value = new_value;
    }
};

struct Creature
{
    Property<int> strength{10};
    Property<int> agility{5};
};

int main()
{
    Creature creature;
    creature.agility = 20;
    auto x = creature.strength;
    cout << x << endl;
}
