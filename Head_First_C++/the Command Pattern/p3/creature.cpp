#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

class Creature
{
    int strength, agility;
public:
    Creature(int strength, int agility):
        strength{strength}, agility{agility} {}
    
    void process_command(const CreatureCommand& cc)
    {
        int *ability;
        switch (cc.ability)
        {
            case CreatureAbility::strength:
                ability = &strength;
                break;
            
            case CreatureAbility::agility:
                ability = &agility;
                break;
        }

        switch (cc.action)
        {
            case CreatureCommand::set:
                *ability = cc.amount;
                break;
            case CreatureCommand::increaseBy:
                *ability += cc.amount;
                break;
            case CreatureCommand::decreaseBy:
                *ability -= cc.amount;
                break;
        }
    }

    int process_query(const CreatureQuery& q) const
    {
        switch(q.ability)
        {
            case CreatureAbility::strength:
                return strength;
            case CreatureAbility::agility:
                return agility;
        }
        return 0;
    }

    void set_strength(int value)
    {
        process_command(CreatureCommand{
            CreatureCommand::set, CreatureAbility::strength, value
        });
    }

    int get_strength() const
    {
        return process_query(CreatureQuery{
            CreatureAbility::strength
        });
    }
};

enum class CreatureAbility {strength, agility};

struct CreatureCommand
{
    enum Action {set, increaseBy, decreaseBy} action;
    CreatureAbility ability;
    int amount;
};

struct CreatureQuery
{
    CreatureAbility ability;
};
