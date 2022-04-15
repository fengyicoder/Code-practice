#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class PersonBuilder;
class PersonAddressBuilder;
class PersonJobBuilder;

class Person
{
    friend PersonBuilder;
    friend PersonAddressBuilder;
    friend PersonJobBuilder;
    string street_address, post_code, city;
    string company_name, position;
    int annual_income = 0;
    Person() {}

public:
    static PersonBuilder create();
};

class PersonBuilderBase
{
protected:
    Person &person;
    explicit PersonBuilderBase(Person &person) : person{person} {}

public:
    operator Person() { return move(person); }
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase
{
    Person p;

public:
    PersonBuilder() : PersonBuilderBase{p} {}
};

class PersonAddressBuilder : public PersonBuilderBase
{
    typedef PersonAddressBuilder self;

public:
    explicit PersonAddressBuilder(Person &person) : PersonBuilderBase{person} {}
    self &at(string street_address)
    {
        person.street_address = street_address;
        return *this;
    }

    self &with_postcode(string post_code)
    {
        person.post_code = post_code;
        return *this;
    }
};

class PersonJobBuilder : public PersonBuilderBase
{
    typedef PersonJobBuilder self;

public:
    explicit PersonJobBuilder(Person &person) : PersonBuilderBase{person} {}
    self &at(string company_name)
    {
        person.company_name = company_name;
        return *this;
    }

    self &earning(int annual_income)
    {
        person.annual_income = annual_income;
        return *this;
    }
};

PersonBuilder Person::create()
{
    static PersonBuilder p;
    return p;
}

PersonAddressBuilder PersonBuilderBase::lives() const
{
    return PersonAddressBuilder{person};
}
PersonJobBuilder PersonBuilderBase::works() const
{
    return PersonJobBuilder{person};
}

int main()
{
    PersonBuilder p = Person::create();
    return 0;
}