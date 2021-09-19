#ifndef __SIMPLEREMOTECONTROL_H__
#define __SIMPLEREMOTECONTROL_H__

class SimpleRemoteControl
{
public:
    void setCommand(Command *command)
    {
        slot = command;
    }
    void buttonWasPressed()
    {
        slot->execute();
    }

public:
    Command *slot;
};

#endif