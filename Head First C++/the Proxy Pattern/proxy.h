#ifndef __PROXY_H__
#define __PROXY_H__

#include "handler.h"
#include "realImage.h"

class Proxy: public Image {
public:
    Proxy() {
        Image* image = new RealImage();
        handler = new Handler(image);
    }
    virtual ~Proxy() {
        delete handler;
    }
    void draw() {
        handler->getInstance()->draw();
    }
    void refresh() {
        handler->getInstance()->refresh();
    }
public:
    Handler *handler;
};

#endif