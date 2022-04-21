#include <string>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;
#include <boost/any.hpp>
using namespace boost;

template<typename T>
struct Observer
{
    virtual void field_changed(
        T &source, const string &field_name
    ) = 0;
}


template<typename T>
struct Observable
{
    void notity(T& source, const string& name);
    void subscrible(Observer<T>* f)
    {
        scoped_lock<mutex> lock{mtx};
        observers.emplace_back(f);
    }
    void unsubscrible(Observer<T>* f)
    {
        scoped_lock<mutex> lock{mtx};
        observers.erase(remove(observers.begin(), observers.end(), observer),
                  observers.end());
    }

    void notify(T &source, const string& name)
    {
        scoped_lock<mutex> lock{mtx};
        for (auto &&obs: observers) obs->field_changed(source, name);
    }
private:
    mutex mtx;
    vector<Observer<T*>> observers;
};

struct Person: Observable<Person>
{
    int get_age() const {
        return age;
    }

    void set_age(const int value)
    {
        if (this->age != age) {
            this->age = age;
            notify(*this, "age");
        }
    }

private:
    int age;
};

struct ConsolePersonObserver: Observer<Person>
{
    void field_changed(Person &source, const string &field_name) override
    {
        cout << "Person's " << field_name << " has changed to " << source.get_age()
         << ".\n";
    }
};

int main()
{
    Person p{ 20 };
    ConsolePersonObserver cpo;
    p.subscribe(&cpo);
    p.set_age(21); // Person's age has changed to 21.
    p.set_age(22); // Person's age has changed to 22.
}