#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

using namespace std;

class Creature {
    enum Abilities{str, alg, intl, count};
    std::array<int, count> abilities;

public:
    int get_strength() const { return abilities[str];}
    void set_strength(int value) {
        abilities[str] = value;
    }
    int get_agility() const { return abilities[alg];}
    void set_agility(int value) {
        abilities[alg] = value;
    }
    int get_intelligence() const { return abilities[intl];}
    void set_intelligence(int value) {
        abilities[intl] = value;
    }

    int sum() const {
        return accumulate(abilities.begin(), abilities.end(), 0);
    }

    double average() const {
        return sum() / (double)count;
    }

    int max() const {
        return *max_element(abilities.begin(), abilities.end());
    }
};