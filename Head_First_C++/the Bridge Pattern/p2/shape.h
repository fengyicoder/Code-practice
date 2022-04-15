#include "renderer.h"

struct Shape
{
protected:
    Renderer& renderer;
    Shape(Renderer& renderer): renderer{renderer} {}
public:
    virtual void draw() = 0;
    virtual void resize(float factor) = 0;
};

struct Circle: Shape
{
    float x, y, radius;
    void draw() override {
        renderer.render_circle(x, y, radius);
    }

    void resize(float factor) override {
        radius *= factor;
    }

    Circle(Renderer& renderer, float x, float y, float radius):
        Shape{renderer}, x{x}, y{y}, radius{radius} {}
};

