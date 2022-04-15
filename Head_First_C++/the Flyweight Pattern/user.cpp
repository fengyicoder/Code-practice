#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

#include <boost/bimap.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
using namespace boost;
using namespace flyweights;

using namespace std;

typedef uint32_t key;

struct User
{
    User(const string& first_name, const string& last_name):
        first_name{add(first_name)}, last_name{add(last_name)} {}

    const string& get_first_name() const {
        return name.left.find(first_name)->second;
    }

    const string& get_last_name const {
        return name.left.find(last_name)->second;
    }

    friend ostream& operator<<(ostream& os, const User& obj) {
        return os << "first_name: " << obj.get_first_name()
                << " last_name: " << obj.get_last_name();
    }

protected:
    key first_name, last_name;
    static bimap<key, string> names;
    static key seed;
    static key add(const string& s) {
        auto it = names.right.find(s);
        if (it == names.right.end()) {
            names.insert({++seed, s});
            return seed;
        }
        return it->second;
    }
};

//使用Boost库中提供的boost::flyweight
struct User2
{
    flyweight<string> first_name, last_name;
    User2(const string& first_name, const string& last_name):
        first_name{first_name}, last_name{last_name} {}
};

int main() {
    User2 john_doe{ "John", "Doe" };
    User2 jane_doe{ "Jane", "Doe" };
    cout << boolalpha <<
    (&jane_doe.last_name.get() == &john_doe.last_name.get());
    return 0;
}