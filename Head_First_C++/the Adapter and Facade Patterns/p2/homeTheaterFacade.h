#ifndef __HOMETHEATERFACADE_H__
#define __HOMETHEATERFACADE_H__

#include <string>
#include "dvdPlayer.h"
#include "screen.h"

using namespace std;


class HomeTheaterFacade {
public:
    HomeTheaterFacade(DvdPlayer* dvd, Screen* screen) {
        this->dvd = dvd;
        this->screen = screen;
    }
    void watchMovie() {
        screen->down();
        dvd->play();
    }
    void endMovie() {
        screen->up();
        dvd->stop();
    }

public:
    DvdPlayer *dvd;
    Screen *screen;
};

#endif