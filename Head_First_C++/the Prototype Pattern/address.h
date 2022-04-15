#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Address
{
    string street, city;
    int suite;

public:
    Address(const string &street, const string &city, const int suite) : street{street}, city{city}, suite{suite} {}
};

struct Contact
{
    string name;
    Address *address;

public:
    Contact() : name{""}, address{nullptr} {}
    Contact(const Contact &other) : name{other.name}, address{new Address{*other.address}} {}
};