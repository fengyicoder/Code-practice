#ifndef __DVDPLAYER_H__
#define __DVDPLAYER_H__

#include <iostream>

using namespace std;


class DvdPlayer
{
public:
    void play()
    {
        cout << "start to play a movie" << endl;
    }
    void stop()
    {
        cout << "stop to play a movie" << endl;
    }
};


#endif