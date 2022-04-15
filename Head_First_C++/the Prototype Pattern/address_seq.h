#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

struct Address
{
    string street, city;
    int suite;

private:
    friend class boost::serialization::serialization::access;
    template <class Ar>
    void serialize(Ar& ar, const unsigned int version) {
        ar& street;
        ar& city;
        ar& suite;
    }
};

struct Contact
{
    string name;
    Address *address=nullptr;

private:
    friend class boost::serialization::serialization::access;
    template <class Ar>
    void serialize(Ar& ar, const unsigned int version) {
        ar& street;
        ar& address;
    }
};

auto clone = [](const Contact& c) {
  // 1. Serialize the contact
  ostringstream oss;
  boost::archive::text_oarchive oa(oss);
  oa << c;
  string s = oss.str();
  // 2. Deserialize the contact
  istringstream iss(oss.str());
  boost::archive::text_iarchive ia(iss);
  Contact result;
  ia >> result;
  return result;
};