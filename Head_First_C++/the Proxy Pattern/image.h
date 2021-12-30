#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <iostream>

using namespace std;

class Image
{
public:
    virtual void draw() = 0;
    virtual void refresh() = 0;
};

#endif