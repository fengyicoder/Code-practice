#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <iostream>
#include "command.h"

using namespace std;

class Light
{
public:
    void on() { cout << "Open the light!" << endl; }
    void off() { cout << "Stop the light!" << endl; }
};

class LightOnCommand : public Command
{
public:
    LightOnCommand(Light *light)
    {
        this->light = light;
    }
    void execute()
    {
        light->on();
    }

public:
    Light *light;
};

class LightOffCommand : public Command
{
public:
    LightOffCommand(Light *light)
    {
        this->light = light;
    }
    void execute()
    {
        light->off();
    }

public:
    Light *light;
};

#endif