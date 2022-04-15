#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Point {
protected:
    Point(const float x, const float y): x{x}, y{y} {}
public:
    static Point NewCartesian(float x, float y) {
        return {x, y};
    }
    static Point NewPloar(float r, float theta) {
        return {r*cos(theta), r*sin(theta)};
    }
private:
    float x;
    float y;
};