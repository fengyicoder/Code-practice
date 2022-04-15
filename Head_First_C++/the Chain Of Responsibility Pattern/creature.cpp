#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

struct Creature
{
    string name;
    int attack, defence;
    Creature(const string &name, const int attack, const int defense)
        : name(name),
          attack(attack),
          defence(defense)
    {
    }

    friend ostream &operator<<(ostream &os, const Creature &obj)
    {
        return os
               << "name: " << obj.name
               << " attack: " << obj.attack
               << " defense: " << obj.defence;
    }
};

class CreatureModifier
{
    CreatureModifier *next{nullptr};

protected:
    Creature &creature;

public:
    explicit CreatureModifier(Creature &creature) : creature(creature) {}
    void add(CreatureModifier *cm)
    {
        if (next)
            next->add(cm);
        else
            next = cm;
    }

    virtual void handle()
    {
        if (next)
            next->handle();
    }
};

class DoubleAttackModifier : public CreatureModifier
{
public:
    explicit DoubleAttackModifier(Creature &creature) : CreatureModifier(creature) {}
    void handle() override
    {
        if (creature.attack <= 2)
            creature.attack += 1;
        CreatureModifier::handle();
    }
};

class NoBonusesModifier : public CreatureModifier
{
    public:
    explicit NoBonusesModifier(Creature& creature) : CreatureModifier(creature) {}

    void handle() override
    {

    }
};

int main()
{
    Creature goblin{"Goblin", 1, 1};
    CreatureModifier root{goblin};
    DoubleAttackModifier r1{goblin};
    DoubleAttackModifier r1_2{goblin};
    root.add(&r1);
    root.add(&r1_2);
    root.handle();
    cout << goblin << endl;
}
