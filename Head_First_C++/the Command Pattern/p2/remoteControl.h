#ifndef __REMOTECONTROL_H__
#define __REMOTECONTROL_H__
#include <iostream>
#include <vector>
#include <string>
#include "command.h"

using namespace std;

class RemoteControl
{
public:
    RemoteControl() {
        onCommands = vector<Command*>(7);
        offCommands = vector<Command*>(7);

        Command *noCommand = new NoCommand();
        for (int i=0; i<7; i++)
        {
            onCommands[i] = noCommand;
            offCommands[i] = noCommand;
        }

    }
    void setCommand(int slot, Command *onCommand, Command *offCommand)
    {
        onCommands[slot] = onCommand;
        offCommands[slot] = offCommand;
    }
    void onButtonWasPressed(int slot)
    {
        onCommands[slot]->execute();
    }
    void offButtonWasPressed(int slot)
    {
        offCommands[slot]->execute();
    }

public:
    vector<Command*> onCommands;
    vector<Command*> offCommands;
};

#endif