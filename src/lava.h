
#ifndef __LAVA_AGENT__H
#define __LAVA_AGENT__H

#include "enviro.h"

#include <stdio.h>

using namespace enviro;



class LavaController : public Process, public AgentInterface
{

public:
    LavaController() : Process(), AgentInterface() {}

    void init()
    {
        prevent_rotation();
    }
    void start() {}

    void update()
    {
        omni_apply_force(0, 500);
    }

    void stop() {}

};

class Lava : public Agent
{
public:
    Lava(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    LavaController c;
};

DECLARE_INTERFACE(Lava)

#endif