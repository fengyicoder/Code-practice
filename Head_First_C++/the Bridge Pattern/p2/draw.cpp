#include "shape.h"

int main() {
    RasterRenderer rr;
    Circle raster_circle{rr, 5, 5, 5};
    raster_circle.draw();
    raster_circle.resize(2);
    raster_circle.draw();
    return 0;
}