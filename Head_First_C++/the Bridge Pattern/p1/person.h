#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Person
{
    std::string name;
    void greet();
    Person();
    ~Person();

    class PersonImpl;
    PersonImpl* impl;
};
