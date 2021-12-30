#ifndef __REALIMAGE_H__
#define __REALIMAGE_H__

#include "image.h"

class RealImage: public Image
{
public:
    void draw() {
        cout << "I am drawing" << endl;
    }
    void refresh() {
        cout << "I am refresh" << endl;
    }
};

#endif