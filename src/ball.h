#ifndef __BALL_AGENT__H
#define __BALL_AGENT__H

#include "enviro.h"
#include <stdio.h>


#define PI 3.14159265
#define to_deg 180 / PI;
#define to_rad PI / 180;

using namespace enviro;

class BallController : public Process, public AgentInterface
{

public:
    BallController() : Process(), AgentInterface(), vx(5) {}

    void init() {

        teleport(x(), y(), angle() + PI/4);
        // if (sensor_value(0) < 5)
        // {
        //     // std::cout << "why not" << std::endl;
        //     // vx = -vx;
        // }
    }

    void start() {}

    void update()
    {

        if (sensor_value(0) < front_sens_mag || sensor_value(1) < front_sens_mag * 0.965 || sensor_value(2) < front_sens_mag * 0.965)
        {
            impact = true;
            double active_side;
            double mid = sensor_value(0);
            bounce_angle = 0;


            if(sensor_value(1) < sensor_value(2))
            {
                active_side = sensor_value(1);
                std::cout << "SENSOR 1" << angle() << std::endl;
                bounce_angle = -get_col_angle(mid, active_side);
            } 
            else 
            {
                active_side = sensor_value(2);
                bounce_angle = get_col_angle(mid, active_side);
            }

        }

        if (impact){
            std::cout << "yes?" << std::endl;
            teleport(x(), y(), angle()+bounce_angle *2);
            impact = false;
        }

        

        track_velocity( 5,  0);
        

        //std::cout << sensor_value(0) << std::endl;
        //std::cout << "Angle : " << angle() << std::endl;
        //omni_apply_force(fx, 0);


        
    }

    double get_col_angle(const double &c_vec, const double &s_vec)
    {
        // SAS triangle, trying to find last side then angle of cent vec to last side
        // Should return a colision angle so we can mirror for projectile reflection
        // c_vec -> a, s_vec -> b, find c and beta
        // gamma is given, the angle between center sensor and either sensor to the left or right, which ever is shortest'
        // probability that a head on collision happens is incredibly low for two float values
        // Returns collision angle in radians

        // double to_deg = 180 / PI;
        // double to_rad = PI / 180;

        double gamma = 30 * to_rad;
        double alpha, beta, c;

        c = sqrt(pow(c_vec, 2) + pow(s_vec, 2) - 2 * c_vec * s_vec * cos(gamma));

        alpha = acos((pow(s_vec, 2) + pow(c, 2) - pow(c_vec, 2)) / (2 * s_vec * c));

        beta = PI - alpha - gamma;

        return beta;
    }

    void stop() {}
    double vx;
    double front_sens_mag = 7;
    bool impact = false;
    double bounce_angle;
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