
#ifndef __BLOCK_AGENT__H
#define __BLOCK_AGENT__H

#include "enviro.h"

#include <stdio.h>

using namespace enviro;

class BlockController : public Process, public AgentInterface
{

public:
    BlockController() : Process(), AgentInterface() {}

    void init()
    {
        prevent_rotation();
    }
    void start() {}

    void update()
    {
        //omni_apply_force(0, 500);
    }

    void stop() {}
};

class Block : public Agent
{
public:
    Block(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    BlockController c;
};

DECLARE_INTERFACE(Block)

#endif