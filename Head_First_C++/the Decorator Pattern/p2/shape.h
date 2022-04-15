#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

//动态装饰器
struct Shape
{
    virtual std::string str() const = 0;
};

struct Circle : Shape
{
    float radius;
    explicit Circle(const float radius) : radius{radius} {};
    void resize(float factor) { radius *= factor; }
    std::string str() const override
    {
        std::ostringstream oss;
        oss << "A circle of radius " << radius;
        return oss.str();
    }
};

struct ColoredShape : Shape
{
    Shape &shape;
    std::string color;
    ColoredShape(Shape &shape, const string &color) : shape(shape), color(color) {}
    std::string str() const override
    {
        std::ostringstream oss;
        oss << shape.str() << " has the color " << color;
        return oss.str();
    }
};

struct TransparentShape : Shape
{
    Shape &shape;
    uint8_t transparency;
    TransparentShape(Shape &shape, const uint8_t transparency) : shape(shape), transparency(transparency){};
    std::string str() const override
    {
        std::ostringstream oss;
        oss << shape.str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};

//静态装饰器
template <typename T>
struct ColoredShape2 : T
{
    static_assert(is_base_of<Shape, T>::value, "Template argument must be a Shape");
    string color;
    template <typename... Args>
    ColoredShape2(const string &color, Args... args) : T(std::forward<Args>(args)...), color(color) {}
    string str() const override
    {
        ostringstream oss;
        oss << T::str() << " has the color " << color;
        return oss.str();
    }
};

template <typename T>
struct TransparentShape2 : T
{
    uint8_t transparency;
    template <typename... Args>
    TransparentShape2(const uint8_t transparency, Args... args) : T(std::forward<Args>(args)...), transparency(transparency){};
    std::string str() const override
    {
        std::ostringstream oss;
        oss << T::str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};
