//*****************************************************************************
//   This program is a 'remote' 3D-collision detector for two balls on linear
//   trajectories and returns, if applicable, the location of the collision for
//   both balls as well as the new velocity vectors (assuming a partially elastic
//   collision as defined by the restitution coefficient).
//
//   All variables apart from 'error' are of Double Precision Floating Point type.
//
//   The Parameters are:
//
//    R    (restitution coefficient)  between 0 and 1 (1=perfectly elastic collision)
//    m1    (mass of ball 1)
//    m2    (mass of ball 2)
//    r1    (radius of ball 1)
//    r2    (radius of ball 2)
//  & x1    (x-coordinate of ball 1)
//  & y1    (y-coordinate of ball 1)
//  & z1    (z-coordinate of ball 1)
//  & x2    (x-coordinate of ball 2)
//  & y2    (y-coordinate of ball 2)
//  & z2    (z-coordinate of ball 2)
//  & vx1   (velocity x-component of ball 1)
//  & vy1   (velocity y-component of ball 1)
//  & vz1   (velocity z-component of ball 1)
//  & vx2   (velocity x-component of ball 2)
//  & vy2   (velocity y-component of ball 2)
//  & vz2   (velocity z-component of ball 2)
//  & error (int)     (0: no error
//                     1: balls do not collide
//                     2: initial positions impossible (balls overlap))
//
//   Note that the parameters with an ampersand (&) are passed by reference,
//   i.e. the corresponding arguments in the calling program will be updated
//   (the positions and velocities however only if 'error'=0).
//   All variables should have the same data types in the calling program
//   and all should be initialized before calling the function.
//
//   This program is free to use for everybody. However, you use it at your own
//   risk and I do not accept any liability resulting from incorrect behaviour.
//   I have tested the program for numerous cases and I could not see anything
//   wrong with it but I can not guarantee that it is bug-free under any
//   circumstances.
//
//   I would appreciate if you could report any problems to me
//   (for contact details see  http://www.plasmaphysics.org.uk/feedback.htm ).
//
//   Thomas Smid   February 2004
//                 December 2005 (a few minor changes to improve speed)
//                 December 2009 (generalization to partially inelastic collisions)
//                 July     2011 (fix for possible rounding errors)
//******************************************************************************

#include <math.h>
#include "coll3d.h"

/*void collision3D(double R, double m1, double m2, double r1, double r2, double *x1, double *y1, double *z1, double* x2, double* y2,
        double* z2, double* vx1, double* vy1, double* vz1, double* vx2, double* vy2, double* vz2, int* error)*/
void collision3D(struct Particle *a, struct Particle *b, int *error)
{

    float pi, r12, m21, d, v, theta2, phi2, st, ct, sp, cp, vx1r, vy1r, vz1r, fvz1r, thetav, phiv, dr, alpha, beta, sbeta, cbeta,
            t, p, dvz2, vx2r, vy2r, vz2r, x21, y21, z21, vx21, vy21, vz21, vx_cm, vy_cm, vz_cm;

//     **** initialize some variables ****
    pi = acos(-1.0E0);
    *error = 0;
    r12 = a->r + b->r;
    m21 = b->m / a->m;
    x21 = b->x - a->x;
    y21 = b->y - a->y;
    z21 = b->z - a->z;
    vx21 = b->vx - a->vx;
    vy21 = b->vy - a->vy;
    vz21 = b->vz - a->vz;

    vx_cm = (a->m * a->vx + b->m * b->vx) / (a->m + b->m);
    vy_cm = (a->m * a->vy + b->m * b->vy) / (a->m + b->m);
    vz_cm = (a->m * a->vz + b->m * b->vz) / (a->m + b->m);

//     **** calculate relative distance and relative speed ***
    d = sqrt(x21 * x21 + y21 * y21 + z21 * z21);
    v = sqrt(vx21 * vx21 + vy21 * vy21 + vz21 * vz21);

//     **** return if distance between balls smaller than sum of radii ****
    if (d < r12)
    {
        *error = 2;
        return;
    }

//     **** return if relative speed = 0 ****
    if (v == 0)
    {
        *error = 1;
        //return;
    }

//     **** shift coordinate system so that ball 1 is at the origin ***
    b->x = x21;
    b->y = y21;
    b->z = z21;

//     **** boost coordinate system so that ball 2 is resting ***
    a->vx = -vx21;
    a->vy = -vy21;
    a->vz = -vz21;

//     **** find the polar coordinates of the location of ball 2 ***
    theta2 = acos(b->z / d);
    if (b->x == 0 && b->y == 0)
    {
        phi2 = 0;
    }
    else
    {
        phi2 = atan2(b->y, b->x);
    }
    st = sin(theta2);
    ct = cos(theta2);
    sp = sin(phi2);
    cp = cos(phi2);

//     **** express the velocity vector of ball 1 in a rotated coordinate
//          system where ball 2 lies on the z-axis ******
    vx1r = ct * cp * a->vx + ct * sp * a->vy - st * a->vz;
    vy1r = cp * a->vy - sp * a->vx;
    vz1r = st * cp * a->vx + st * sp * a->vy + ct * a->vz;
    fvz1r = vz1r / v;
    if (fvz1r > 1)
    {
        fvz1r = 1;
    }   // fix for possible rounding errors
    else if (fvz1r < -1)
    {
        fvz1r = -1;
    }
    thetav = acos(fvz1r);
    if (vx1r == 0 && vy1r == 0)
    {
        phiv = 0;
    }
    else
    {
        phiv = atan2(vy1r, vx1r);
    }

//     **** calculate the normalized impact parameter ***
    dr = d * sin(thetav) / r12;

//     **** return old positions and velocities if balls do not collide ***
    if (thetav > pi / 2 || fabs(dr) > 1)
    {
        b->x = b->x + a->x;
        b->y = b->y + a->y;
        b->z = b->z + a->z;
        a->vx = a->vx + b->vx;
        a->vy = a->vy + b->vy;
        a->vz = a->vz + b->vz;
        *error = 1;
        return;
    }

//     **** calculate impact angles if balls do collide ***
    alpha = asin(-dr);
    beta = phiv;
    sbeta = sin(beta);
    cbeta = cos(beta);

//     **** calculate time to collision ***
    t = (d * cos(thetav) - r12 * sqrt(1 - dr * dr)) / v;

//     **** update positions and reverse the coordinate shift ***
    b->x = b->x + b->vx * t + a->x;
    b->y = b->y + b->vy * t + a->y;
    b->z = b->z + b->vz * t + a->z;
    a->x = (a->vx + b->vx) * t + a->x;
    a->y = (a->vy + b->vy) * t + a->y;
    a->z = (a->vz + b->vz) * t + a->z;

//  ***  update velocities ***

    p = tan(thetav + alpha);

    dvz2 = 2 * (vz1r + p * (cbeta * vx1r + sbeta * vy1r)) / ((1 + p * p) * (1 + m21));

    vz2r = dvz2;
    vx2r = p * cbeta * dvz2;
    vy2r = p * sbeta * dvz2;
    vz1r = vz1r - m21 * vz2r;
    vx1r = vx1r - m21 * vx2r;
    vy1r = vy1r - m21 * vy2r;

//     **** rotate the velocity vectors back and add the initial velocity
//           vector of ball 2 to retrieve the original coordinate system ****

    a->vx = ct * cp * vx1r - sp * vy1r + st * cp * vz1r + b->vx;
    a->vy = ct * sp * vx1r + cp * vy1r + st * sp * vz1r + b->vy;
    a->vz = ct * vz1r - st * vx1r + b->vz;
    b->vx = ct * cp * vx2r - sp * vy2r + st * cp * vz2r + b->vx;
    b->vy = ct * sp * vx2r + cp * vy2r + st * sp * vz2r + b->vy;
    b->vz = ct * vz2r - st * vx2r + b->vz;

//     ***  velocity correction for inelastic collisions ***

    a->vx = (a->vx - vx_cm) * CONST_RESTITUTION + vx_cm;
    a->vy = (a->vy - vy_cm) * CONST_RESTITUTION + vy_cm;
    a->vz = (a->vz - vz_cm) * CONST_RESTITUTION + vz_cm;
    b->vx = (b->vx - vx_cm) * CONST_RESTITUTION + vx_cm;
    b->vy = (b->vy - vy_cm) * CONST_RESTITUTION + vy_cm;
    b->vz = (b->vz - vz_cm) * CONST_RESTITUTION + vz_cm;

    return;
}
