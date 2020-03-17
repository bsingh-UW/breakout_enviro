#include <iostream>
#include "ball.h"
#include <math.h>
#include <vector>

// #define PI 3.14159265
// #define to_deg 180/PI;
// #define to_rad PI/180;

using namespace enviro;

// Put your implementations here

using namespace std;

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
    double alpha,beta,c;

    c = sqrt(pow(c_vec, 2) + pow(s_vec, 2) - 2 * c_vec * s_vec * cos(gamma));

    alpha = acos( (pow(s_vec, 2) + pow(c, 2) - pow(c_vec, 2)) / (2*s_vec*c)) ;

    beta = PI - alpha - gamma;

    return beta;
}

// vector<double> get_col_vector(const double &col_angle, double &angle, const double &x, const double &y)
// {
//     vector<double> dest{0,0};
//     // Find current vector direction

//     dest = dest
//     dest
// }