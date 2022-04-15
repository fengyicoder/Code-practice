#include "person.h"
#include "stdio.h"

struct Person::PersonImpl
{
    void greet(Person* p);
};

Person::Person(): impl(new PersonImpl) {}

Person::~Person() {
    delete impl;
}

void Person::greet() {
    impl->greet(this);
}

void Person::PersonImpl::greet(Person* p) {
    printf("hello %s", p->name.c_str());
}