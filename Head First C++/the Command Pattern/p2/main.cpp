
#include <vector>
#include "command.h"
#include "light.h"
#include "stereo.h"
#include "remoteControl.h"

int main()
{
    RemoteControl *remote = new RemoteControl();
    Light *light = new Light();
    LightOnCommand *lightOn = new LightOnCommand(light);
    LightOffCommand *lightOff = new LightOffCommand(light);
    Stereo *stereo = new Stereo();
    StereoOnCommand *stereoOn = new StereoOnCommand(stereo);
    StereoOffCommand *stereoOff = new StereoOffCommand(stereo);
    remote->setCommand(0, lightOn, lightOff);
    remote->setCommand(1, stereoOn, stereoOff);
    remote->onButtonWasPressed(0);
    remote->offButtonWasPressed(0);
    remote->onButtonWasPressed(1);
    remote->offButtonWasPressed(1);

    return 0;
}