
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
        /*  
            Movement for the player block. Straight forward, uses arrow keys. 

            A stop flag is created so the player stops moving once the key stops
            being pressed, causes a damp_movement() to be called. 
        */
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

        /*  
            Movement functions for the block. Uses same physics from 
            the platformer, other than the damp_movement().
            
        */

        if (RIGHT)
        {
            vx = 0.2 * VEL_X;
            fx = 0;
            fx = -K_X * (velocity().x - vx);
            omni_apply_force(fx, G);
        }
        if (LEFT)
        {
            vx = -0.2 * VEL_X;
            fx = 0;
            fx = -K_X * (velocity().x - vx);
            omni_apply_force(fx, G);
        }
        if(STOP){
            damp_movement();
        }

        /*  
            These methods are for keeping score. Since the display is attached to the player
            , I added event watchers. I set the watches to flip flags, since the scores are 
            fixed. Also I was having issues with json. 
            
        */

        watch("hit_minus", [this](Event e) {
            hit_minus = true;
        });

        watch("hit_plus", [this](Event e) {
            hit_plus = true;
        });



        /*  
            Adds or subtracs points based off of emit events. Booleans used because
            the watch method would trigger multiple times before reaching the display. 
        */

        if(hit_minus){
            score = score - 50;
            hit_minus = false;
        }
        if (hit_plus)
        {
            score = score + 10;
            hit_plus = false;
        }

        /*  
            Adds casted score to the display string and gets displayed by label.  
        */

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