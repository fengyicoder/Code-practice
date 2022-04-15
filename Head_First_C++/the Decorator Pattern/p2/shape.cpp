#include "shape.h"

int main() {
    ColoredShape2<TransparentShape2<Circle>> sq("red", 51, 5.0);
    std::cout << sq.str() << std::endl;
    return 0;
}