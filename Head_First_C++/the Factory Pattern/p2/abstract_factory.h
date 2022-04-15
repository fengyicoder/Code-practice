#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct HotDrink
{
    virtual void prepare(int volume) = 0;
};

struct Tea: HotDrink
{
    void prepare(int volume) override
    {
        cout << "Take tea bag, boil water, pour "
            << volume
            << "ml, add some lemon" << endl;
    }
};

struct Coffee: HotDrink
{
    void prepare(int volume) override
    {
        cout << "Take coffe bag, boil water, pour "
            << volume << endl;
    }
};

struct HotDrinkFactory {
    virtual unique_ptr<HotDrink> make() const = 0;
};

struct CoffeeFactory: HotDrinkFactory {
    unique_ptr<HotDrink> make() const override
    {
        return make_unique<Coffee>();
    }
};

struct TeaFactory: HotDrinkFactory {
    unique_ptr<HotDrink> make() const override
    {
        return make_unique<Tea>();
    }
};

class DrinkFactory
{
    map<string, unique_ptr<HotDrinkFactory>> hot_factories;
    DrinkFactory()
    {
        hot_factories['coffee'] = make_unique<CoffeeFactory>();
        hot_factories['tea'] = make_unique<TeaFactory>();
    }

    unique_ptr<HotDrink> make_drink(const string& name)
    {
        auto drink = hot_factories[name]->make();
        drink->prepare(200);
        return drink;
    }
};

//函数工厂
class DrinkWithVolumeFactory
{
    map<string, function<unique_ptr<HotDrink>()>> factories;
public:
    DrinkWithVolumeFactory()
    {
        factories['tea'] = []{
            auto tea = make_unique<Tea>();
            tea->prepare(200);
            return tea;
        }

        factories['coffee'] = []{
            auto coffee = make_unique<Coffee>();
            Coffee->prepare(200);
            return coffee;
        }
    }
};


