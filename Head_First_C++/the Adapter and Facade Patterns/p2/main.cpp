#include "homeTheaterFacade.h"

int main() {
    DvdPlayer *dvd = new DvdPlayer();
    Screen *screen = new Screen();
    HomeTheaterFacade *homeTheaterFacade = new HomeTheaterFacade(dvd, screen);
    homeTheaterFacade->watchMovie();
    homeTheaterFacade->endMovie();
}