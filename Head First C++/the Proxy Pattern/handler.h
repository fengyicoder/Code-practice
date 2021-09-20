#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "image.h"

class Handler
{
public:
    Handler(Image *image) {
        this->image = image;
    }
    virtual ~Handler() {}
    Image* getInstance() {
        return image;
    }
public:
    Image *image;

};

#endif