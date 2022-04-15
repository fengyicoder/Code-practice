#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

struct Renderer
{
    virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer: Renderer
{
    void render_circle(float x, float y, float radius) override {
        std::cout << "Rasterizing circle of radius" << radius << std::endl; 
    }
};

struct RasterRenderer: Renderer
{
    void render_circle(float x, float y, float radius) override {
        std::cout << "Drawing a vector circle of radius" << radius << std::endl; 
    }
};




