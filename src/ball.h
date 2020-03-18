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
    BallController() : Process(), AgentInterface(){}

    void init() {

        teleport(50, 50, angle() + PI/4);

        notice_collisions_with("Lava", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            std::cout << other_robot_id << std::endl;
            
            if (other_robot_id == 2)
            {
                init();
            }
            else{
                collision();
            }
        });

        notice_collisions_with("Wall", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            collision();
        });

        notice_collisions_with("Player", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            collision();
        });

        notice_collisions_with("Block", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            Agent &other_robot = find_agent(other_robot_id);
            collision();
            remove_agent(other_robot_id);
        });
    }

    void start() {}

    void update()
    {

        if (impact){
            teleport(x(), y(), angle()+bounce_angle *2);
            impact = false;
        }

        track_velocity( 5,  0);

    }

    void collision(){
        impact = true;
        double active_side;
        double mid = sensor_value(0);
        bounce_angle = 0;

        if (sensor_value(1) < sensor_value(2))
        {
            active_side = sensor_value(1);
            bounce_angle = -get_col_angle(mid, active_side);
        }
        else
        {
            active_side = sensor_value(2);
            bounce_angle = get_col_angle(mid, active_side);
        }
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