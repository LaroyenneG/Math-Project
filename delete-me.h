//
// Created by guillaume on 25/12/17.
//

#ifndef MATH_PROJECT_DELETE_ME_H
#define MATH_PROJECT_DELETE_ME_H


/*
 * Ce code normalement stocké dans le fichier d'entête.
 * Il est exceptionnellement placé dans ce fichier en raison des contraintes de remise.
 */

/****************************************************************************************************
 * pendule_22.h
 ***************************************************************************************************/

#define GRAVITY 9.80665 // (m/s^-2)
#define PI 3.141592653589793

#define BAR_TOTAL_LENGTH 1.0    // (m)
#define BAR_DIAMETER 0.01   // (m)
#define BAR_MASS 0.2    // (kg)
#define WEIGHT_HEIGHT 0.07   // (m)
#define OUTER_DIAMETER_WEIGHT 0.06   // (m)
#define DISTANCE_CENTER_WEIGHT_PINTLE 0.95   // (m)
#define MASS_WEIGHT 0.30 // (kg)
#define MASS_TROLLEY 1.20   // (kg)
#define LINEAR_FRICTION_TROLLEY 0.1 // (N/m/s)
#define PIVOT_FRICTION 0.2    // (N*m/rad/s)

#define H 0.01  // (s)


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

point_t buildRotationFriction(double angle, double length, double speedRot, double friction);

point_t buildLinearFriction(double speed, double friction);

void stopSystem(system_t *system);

double distancePoint(point_t point1, point_t point2);

double rotationSpeedEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system);

double linearSpeedEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system);

double *getEquationCoefficients(double angle, double rotationSpeed, double trolleySpeed, system_t system);

/****************************************************************************************************
 * end pendule_22.h
 ***************************************************************************************************/

#endif //MATH_PROJECT_DELETE_ME_H
