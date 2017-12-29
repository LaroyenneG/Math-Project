//
// Created by guillaume on 25/12/17.
//

#ifndef MATH_PROJECT_DELETE_ME_H
#define MATH_PROJECT_DELETE_ME_H

#include "libmat.h"






/*
 * Code normalement stocké dans le fichier d'entête. Il est exceptionnellement placé
 * dans ce fichier en raison des contraintes de remise.
 */

/****************************************************************************************************
 * pendule_22.h
 ***************************************************************************************************/

#define GRAVITY 9.807
#define PI 3.141592653589793

#define BAR_TOTAL_LENGTH 1.0
#define BAR_DIAMETER 0.01
#define BAR_MASS 0.2
#define FLYWEIGHT_HEIGHT 0.07
#define OUTER_DIAMETER_FLYWEIGHT 0.06
#define DISTANCE_CENTER_FLYWEIGHT_PINTLE 0.95
#define MASS_FLYWEIGHT 0.30
#define MASS_CAR 1.20
#define LINEAR_FRICTION_CAR 0.001
#define PINTLE_FRICTION 20.0

#define H 0.01


typedef struct {

    double x;
    double y;

} point_t;


typedef struct {

    point_t position;
    point_t velocityVector;
    double mass;
    double friction;

} trolley_t;

typedef struct {

    point_t a;
    point_t b;
    point_t velocityVector;
    point_t gravityCenter;
    double mass;

} weight_t;

typedef struct {

    point_t position;
    double angle;
    double rotationSpeed;
    double friction;

} pivot_t;

typedef struct {

    point_t a;
    point_t b;
    point_t gravityCenter;
    double length;
    double mass;
    double diameter;

} bar_t;

typedef struct {

    trolley_t trolley;
    pivot_t pivot;
    weight_t weight;
    bar_t bar;
    point_t inertiaCenter;

    point_t pendulumGravityCenter;
    point_t pendulumVelocityVector;

    double mechanicalEnergy;
    double potentialEnergy;
    double kineticEnergy;

    // incertain

    double lengthPendulum;
    double lengthBar;
    double lengthWeight;

} system_t;


system_t buildSystem(double angle, double p0);


double potentialEnergySystem(system_t system);

double kineticEnergySystem(system_t system);

void putPendulumGravityCenterSystem(system_t *system);

point_t interpolatePoint(point_t point1, point_t point2, double f);

point_t rectangleGravityCenter(point_t a, point_t b);

void putAngleSystem(system_t *system);

system_t nextTimeSystem(system_t system);

double vectorNorm(point_t v);

void printLineSystem(system_t system, double time);

system_t rk4System(double h, system_t system);

void putEnergySystem(system_t *system);

void putPivotPositionSystem(system_t *system);

void putPendulumVelocityVectorSystem(system_t *system);

void putInertiaCenterSystem(system_t *system);

point_t applyRotationFriction(double angle, double length, double speedRot, double speedTro, double friction);

point_t applyLinearFriction(double speed, double friction);

double distancePoint(point_t point1, point_t point2);


/****************************************************************************************************
 * end pendule_22.h
 ***************************************************************************************************/








#endif //MATH_PROJECT_DELETE_ME_H
