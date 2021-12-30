#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <iostream>

using namespace std;


class Screen
{
public:
    Screen(){}
    ~Screen(){}

    void down()
    {
        cout << "Screen is down" << endl;
    }
    void up()
    {
        cout << "Screen is up" << endl;
    }
};


#endif

