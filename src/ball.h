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


        /* 
            Moves the object to the start point, at an angle of 45 degrees when called. 
            Nessecary to have the object rotated on start. See issue created by me. 
        */
        teleport(0, 0, angle() + PI/4);




        /* 
            Lava is a kill wall object. Created so I can detect whether the ball collides with the bottom floor.

            I repurposed the lava object for the celing as well, so I didn't have to recreate an object with the same shape. 
            The floor has an id of 2, so if it detects a collision with id 2, restarts the ball and sets a flag to send an event. 
            The else is intended for the celing which is benign and reflects. 

        */
        notice_collisions_with("Lava", [&](Event &e) {
            int other_robot_id = e.value()["id"];

            if (other_robot_id == 2)
            {
                hit_minus = true;
                init();
            }
            else{
                collision();
            }
        });


        /* 
            Side walls, simple agents that dont do anything. Created so I can detect collisions. Reflects the ball on touch by
            calling collision();.
        */
        notice_collisions_with("Wall", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            collision();
        });


        /* 
            The player block. Reflects the ball on touch by
            calling collision();.
        */
        notice_collisions_with("Player", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            collision();
        });


        /* 
           Game blocks, performs the same collision as other objects. On a colision, causes the reflection. 
           Then removes the block via remove_agent();. Sets a flag to add positive points, by calling an event lower.
        */
        notice_collisions_with("Block", [&](Event &e) {
            int other_robot_id = e.value()["id"];
            //Agent &other_robot = find_agent(other_robot_id);
            collision();
            remove_agent(other_robot_id);

            hit_plus = true;
        });
    }

    void start() {}



    void update()
    {   
        /*
            If the flags flipped from the notice_collisions_with are active, emits an event. 
            Reason a flag was created, the tick rate was too fast before the the listener could
            react to the event, so it was triggering the listener to perform its actions mutiple times.
            Removes points, triggered by hitting the kill wall (lava)
        */
        if(hit_minus){
            emit(Event("hit_minus", {"points", -100}));
            hit_minus = false;
        }

        /*
            If the flags flipped from the notice_collisions_with are active, emits an event. 
            Reason a flag was created, the tick rate was too fast before the the listener could
            react to the event, so it was triggering the listener to perform its actions mutiple times.
            Adds points, triggered by hitting a block. 
        */
        if (hit_plus)
        {
            emit(Event("hit_plus", {"points", 10}));
            hit_plus = false;
        }

        /*
            Triggered by a collision event. So if the collision() method is called, sets the impact flag to true. 

            All this does perform the reflection. Turning wasn't fast enough for the physics to reflect a ball bouncing.
            So I teleport the ball to the current position it's in, except at a new angle, calculated in get_col_angle()*2;
            get_col_angle() calculates the angle of which the ball is hitting the reflecting object. Then I multiply this angle by 2
            and add it to its current angle. 

            Sets the flag to false when completed so its not always called, even in the update method. 
        */
        if (impact){
            teleport(x(), y(), angle()+bounce_angle *2);
            impact = false;
        }

        /* 
            Since the ball turns instantaneously, I just constantly have it move at a certain velocity, never having
            to actually turn it, kinda sneaky huh. 
        */
        track_velocity( 5,  0);

    }

    /*
        Triggered if notice_collisions_with() with a reflection wall. 

        Sets the self teleport flag "impact" to true, so the ball can be reflected in the update method. 

        Since I need to detect the angle of which the ball hits a wall to reflect it, I need 3 sensors. 
        One for the middle, and one for each side in a symetrical patter. Currently +- 30 degrees. 
        When called, the ball has hit an object. So in that instantaneous moment, I gather the distances of the 
        mid and each side. I choose the side sensor that has a smaller value, since that side is more likely hitting
        a reflectable object. 

        Active side is equal to the smaller sensor side value. Then the bounce angle is calculated given the length
        of the mid sensor and the shorter side value. Sensor value is 1 since its a negative angle turn. 
        


    */
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

    /* 
        To calculate the bounce angle, I used a geometrical method. Since I have the values
        of 2 sensors hitting a wall, and a given angle - the problem then turns into solving for a 
        SAS triangle. The reflect surface being the missing side. 

        https://en.wikipedia.org/wiki/Solution_of_triangles#Two_sides_and_the_included_angle_given_(SAS)

        This method follows that procedure in the link above, to return beta. a is the center sensor and b is the short side sensor. 
    */

    double get_col_angle(const double &c_vec, const double &s_vec)
    {


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
    bool hit_plus = false;
    bool hit_minus = false;
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