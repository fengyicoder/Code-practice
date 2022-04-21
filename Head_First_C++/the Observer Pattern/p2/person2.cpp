#include <iostream>
#include <string>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost;
using namespace signals2;

template<typename T>
struct Observable {
    signal<void(T&, const string&)> property_changed;
};

struct Person: Observable<Person>
{
    void set_age(const int age) {
        if(this->age == age) return;
        this->age = age;
        property_changed(*this, "age");
    }
};

int main() {
    Person p{123};
    auto conn = p.property_changed.connect([](Person&, const string& prop_name) {
    cout << prop_name << " has been changed" << endl;
    });
    p.set_age(20);  // name has been changed
    // later, optionally
    conn.disconnect();
    return 0;
}
