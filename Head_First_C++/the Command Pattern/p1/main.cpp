#include "command.h"
#include "light.h"
#include "simpleRemoteControl.h"

int main()
{
    SimpleRemoteControl *remote = new SimpleRemoteControl();
    Light *light = new Light();
    LightOnCommand *lightOn = new LightOnCommand(light);
    remote->setCommand(lightOn);
    remote->buttonWasPressed();

    return 0;
}