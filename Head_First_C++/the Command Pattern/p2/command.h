#ifndef __COMMAND_H__
#define __COMMAND_H__

class Command {
public:
    virtual void execute() = 0;
};

class NoCommand: public Command {
public:
    void execute() {}
};

#endif