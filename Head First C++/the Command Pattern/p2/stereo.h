#ifndef __STEREO_H__
#define __STEREO_H__
#include <iostream>
#include "command.h"

using namespace std;

class Stereo
{
public:
    void on() { cout << "Open the stereo!" << endl; }
    void off() { cout << "Stop the stereo!" << endl; }
};

class StereoOnCommand : public Command
{
public:
    StereoOnCommand(Stereo *stereo)
    {
        this->stereo = stereo;
    }
    void execute()
    {
        stereo->on();
    }

public:
    Stereo *stereo;
};

class StereoOffCommand : public Command
{
public:
    StereoOffCommand(Stereo *stereo)
    {
        this->stereo = stereo;
    }
    void execute()
    {
        stereo->off();
    }

public:
    Stereo *stereo;
};

#endif