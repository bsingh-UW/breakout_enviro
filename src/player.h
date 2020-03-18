
#ifndef __PLAYER_AGENT__H
#define __PLAYER_AGENT__H

#include "enviro.h"
#include <math.h>
#include <stdio.h>
#include <string>
using namespace enviro;


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

        watch("hit_minus", [this](Event e) {
            hit_minus = true;
        });

        watch("hit_plus", [this](Event e) {
            hit_plus = true;
        });

        if(hit_minus){
            score = score - 50;
            hit_minus = false;
        }
        if (hit_plus)
        {
            score = score + 10;
            hit_plus = false;
        }
        
        display = "Score : " + std::to_string((int)score);
        label(display, -10, 50);
    }

    void stop() {}

    bool LEFT, RIGHT, STOP;
    double vx;
    double score = 0;
    std::string display;
    const double VEL_X = 300;
    const double K_X = 15;
    const double G = 500;
    double now_score;
    bool hit_plus = false;
    bool hit_minus = false;
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