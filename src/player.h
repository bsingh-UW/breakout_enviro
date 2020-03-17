
#ifndef __PLAYER_AGENT__H
#define __PLAYER_AGENT__H

#include "enviro.h"
#include <math.h>
#include <stdio.h>

using namespace enviro;

double min(double a, double b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

class PlayerController : public Process, public AgentInterface
{

public:
    PlayerController() : Process(), AgentInterface(), LEFT(false), RIGHT(false) {}



    void init()
    {
        prevent_rotation();
        watch("keydown", [&](Event &e) {
            std::string k = e.value()["key"];
            if (k == "ArrowLeft")
            {
                LEFT = true;
                STOP = false;
            }
            else if (k == "ArrowRight")
            {
                RIGHT = true;
                STOP = false;
            }
            //std::cout << k << std::endl;
        });
        watch("keyup", [&](Event &e) {
            std::string k = e.value()["key"];
            if (k == "ArrowLeft")
            {
                LEFT = false;
                STOP = true;

            }
            else if (k == "ArrowRight")
            {
                RIGHT = false;
                STOP = true;
            }
        });
    }
    void start() {}

    void update()
    {
        double fx;

        if (RIGHT)
        {
            vx = 0.2 * VEL_X;
            fx = 0;
            fx = -K_X * (velocity().x - vx);
            omni_apply_force(fx, G);
            //std::cout << "RIGHT" << std::endl;
        }
        if (LEFT)
        {
            vx = -0.2 * VEL_X;
            fx = 0;
            fx = -K_X * (velocity().x - vx);
            omni_apply_force(fx, G);
            //std::cout << "LEFT" << std::endl;
        }
        if(STOP){
            damp_movement();
        }

    }

    void stop() {}

    bool LEFT, RIGHT, STOP;
    double vx;

    const double VEL_X = 300;
    const double K_X = 15;
    const double G = 500;
    const double H_MIN = 1.0;
};

class Player : public Agent
{
public:
    Player(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    PlayerController c;
};

DECLARE_INTERFACE(Player)

#endif