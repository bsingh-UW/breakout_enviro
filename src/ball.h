#ifndef __BALL_AGENT__H
#define __BALL_AGENT__H

#include "enviro.h"
#include <stdio.h>

using namespace enviro;

class BallController : public Process, public AgentInterface
{

public:
    BallController() : Process(), AgentInterface(), vx(20) {}

    void init() {
        if (sensor_value(0) < 5)
        {
            std::cout << "why not" << std::endl;
            vx = -vx;
        }
    }

    void start() {}

    void update()
    {

        // omni_apply_force(
        //     fmax,
        //     0);
        if (sensor_value(0) < 5 || sensor_value(1) < front_sens_mag * 0.965 || sensor_value(2) < 5 * 0.965)
        {
            double active_side;
            if(sensor_value(1) < sensor_value(2))
            {
                active_side = sensor_value(1)
            } 
            else 
            {
                active_side = sensor_value(1)
            }
            
            
            
            
            
            double active_side = 
            // std::cout << "why not" << std::endl;
            // vx = -vx;
        }

        double fx = -30 * (velocity().x - vx);

        std::cout << sensor_value(0) << std::endl;
        omni_apply_force(fx, 0);
    }

    void stop() {}
    double vx;
    double front_sens_mag = 5;
    //const int fmax = 100.0;
};

class Ball : public Agent
{
public:
    Ball(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    BallController c;
};

DECLARE_INTERFACE(Ball)

#endif