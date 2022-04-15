#include "graphic.h"

void DrawPoints(CPainDC& dc, std::vector<Point>::iterator start,
    std::vector<Point>::iterator end) {
    for(auto i=start; i != end; ++i) dc.SetPixel(i->x, i->y, 0);
}

int main() {
    return 0;
}