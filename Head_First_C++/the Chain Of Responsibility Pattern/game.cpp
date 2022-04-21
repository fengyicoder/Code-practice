#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"
#include <boost/signals2.hpp>
using namespace boost::signals2;

using namespace std;

struct Game
{
    signal<void (Query&)> queries;
};

struct Query
{
    string creature_name;
    enum Argument {attack, defense} argument;
    int result;
};

class Creature
{
    Game& game;
    int attack, defence;
public:
    string name;
    Creature(Game& game, const string &name, const int attack, const int defense)
        : game(game),
        name(name),
          attack(attack),
          defence(defense)
    {
    }

    friend ostream &operator<<(ostream &os, const Creature &obj)
    {
        return os
               << "name: " << obj.name
               << " attack: " << obj.get_attack()
               << " defense: " << obj.defence;
    }

    int get_attack() const {
        Query q{name, Query::Argument::attack, attack};
        game.queries(q);
        return q.result;
    }
};

class CreatureModifier
{
    Game& game;
    Greature& creature;
public:
    explicit CreatureModifier(Game& game, Creature &creature) : game(game), creature(creature) {}

};

class DoubleAttackModifier: public CreatureModifier
{
    connection conn; 
public:
    DoubleAttackModifier(Game& game, Creature& creature)
        : CreatureModifier(game, creature)
        {
            conn = game.queries.connect([&](Query&q){
                if (q.creature_name == creature.name &&
                    q.argument == Query::Argument::attack)
                    q.result *= 2
                };
            )
        }
        ~DoubleAttackModifier() { conn.disconnect(); }
};

int main(int ac, char* av)
{
    Game game;
    Creature goblin{ game, "Strong Goblin", 2, 2 };

    cout << goblin << endl;

    {
        DoubleAttackModifier dam{ game, goblin };

        cout << goblin << endl;
    }

    cout << goblin << endl;

    getchar();
    return 0;
}