#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

using namespace std;

struct GraphicObject
{
    virtual void draw() = 0;
};

struct Circle : GraphicObject
{
    void draw() override
    {
        std::cout << "Circle" << std::endl;
    }
};

struct Group : GraphicObject
{
    std::string name;
    explicit Group(const std::string &name) : name(name) {}
    void draw() override
    {
        std::cout << "Group " << name.c_str() << " contains:" << std::endl;
        for (auto &&o : objects)
            o->draw();
    }
    std::vector<GraphicObject *> objects;
};
