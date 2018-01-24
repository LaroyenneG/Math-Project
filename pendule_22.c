//
// Created by guillaume on 14/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <zconf.h>


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



/*
 * Returns the interpolate values.
 */
point_t interpolatePoint(point_t point1, point_t point2, double f) {

    point_t result;

    result.x = f * point1.x + (1 - f) * point2.x;
    result.y = f * point1.y + (1 - f) * point2.y;

    return result;
}


/*
 * Returns the distance between two points.
 */
double distancePoint(point_t point1, point_t point2) {

    return sqrt(pow(point1.x - point2.x, 2.0) + pow(point1.y - point2.y, 2.0));
}


/*
 * Returns the norm of vector v.
 */
double vectorNorm(point_t v) {

    return sqrt(pow(v.x, 2.0) + pow(v.y, 2.0));
}


/*
 * Returns the center of gravity of a rectangle.
 */
point_t rectangleGravityCenter(point_t a, point_t b) {

    point_t g;

    g.x = (b.x + a.x) / 2.0;
    g.y = (b.y + a.y) / 2.0;

    return g;
}


/*
 * Returns a system at time t0.
 */
system_t buildSystem(double angle, double p0) {

    system_t system;

    system.trolley.position.x = p0;
    system.trolley.position.y = 0.0;
    system.trolley.mass = MASS_TROLLEY;
    system.trolley.friction = LINEAR_FRICTION_TROLLEY;
    system.trolley.velocityVector.x = 0.0;
    system.trolley.velocityVector.y = 0.0;


    system.pivot.angle = angle * PI / 180;
    system.pivot.rotationSpeed = 0.0;
    system.pivot.friction = PIVOT_FRICTION;
    putPivotPositionSystem(&system);


    system.weight.mass = MASS_WEIGHT;
    system.weight.velocityVector.x = 0.0;
    system.weight.velocityVector.y = 0.0;
    system.weight.a.x = system.pivot.position.x - OUTER_DIAMETER_WEIGHT / 2.0;
    system.weight.a.y = system.pivot.position.y + (DISTANCE_CENTER_WEIGHT_PINTLE - WEIGHT_HEIGHT / 2.0);
    system.weight.b.x = system.pivot.position.x + OUTER_DIAMETER_WEIGHT / 2.0;
    system.weight.b.y = system.pivot.position.y + (DISTANCE_CENTER_WEIGHT_PINTLE + WEIGHT_HEIGHT / 2.0);
    system.weight.gravityCenter = rectangleGravityCenter(system.weight.a, system.weight.b);


    system.bar.mass = BAR_MASS;
    system.bar.diameter = BAR_DIAMETER;
    system.bar.length = BAR_TOTAL_LENGTH;
    system.bar.a.x = system.pivot.position.x - BAR_DIAMETER / 2.0;
    system.bar.a.y = system.pivot.position.y;
    system.bar.b.x = system.pivot.position.x + BAR_DIAMETER / 2.0;
    system.bar.b.y = system.pivot.position.y + system.bar.length;
    system.bar.gravityCenter = rectangleGravityCenter(system.bar.a, system.bar.b);


    putPendulumGravityCenterSystem(&system);

    system.lengthBar = distancePoint(system.pivot.position, system.bar.gravityCenter);
    system.lengthWeight = distancePoint(system.pivot.position, system.weight.gravityCenter);
    system.lengthPendulum = distancePoint(system.pivot.position, system.pendulumGravityCenter);

    putAngleSystem(&system);
    putPendulumGravityCenterSystem(&system);
    putInertiaCenterSystem(&system);
    putPendulumVelocityVectorSystem(&system);
    putEnergySystem(&system);

    return system;
}


/*
 * Writes all energy values into the system.
 */
void putEnergySystem(system_t *system) {

    system->kineticEnergy = kineticEnergySystem(*system);
    system->potentialEnergy = potentialEnergySystem(*system);
    system->mechanicalEnergy = system->potentialEnergy + system->kineticEnergy;
}


/*
 * Writes the gravity center of pendulum into the system structure.
 */
void putPendulumGravityCenterSystem(system_t *system) {

    system->pendulumGravityCenter.x =
            (system->bar.gravityCenter.x * system->bar.mass + system->weight.gravityCenter.x * system->weight.mass) /
            (system->bar.mass + system->weight.mass);

    system->pendulumGravityCenter.y =
            (system->bar.gravityCenter.y * system->bar.mass + system->weight.gravityCenter.y * system->weight.mass) /
            (system->bar.mass + system->weight.mass);
}


/*
 * Writes the center of gravity of the system into the structure.
 */
void putInertiaCenterSystem(system_t *system) {

    system->inertiaCenter = interpolatePoint(system->trolley.position, system->pendulumGravityCenter,
                                             system->trolley.mass /
                                             (system->trolley.mass + system->weight.mass + system->bar.mass));
}


/*
 * Places the centers of gravity according to the angle of the system.
 */
void putAngleSystem(system_t *system) {

    system->weight.gravityCenter.x = system->pivot.position.x + system->lengthWeight * sin(system->pivot.angle);
    system->weight.gravityCenter.y = system->pivot.position.y + system->lengthWeight * cos(system->pivot.angle);

    system->bar.gravityCenter.x = system->pivot.position.x + system->lengthBar * sin(system->pivot.angle);
    system->bar.gravityCenter.y = system->pivot.position.y + system->lengthBar * cos(system->pivot.angle);
}


/*
 * Returns the system at time t + h.
 */
system_t nextTimeSystem(system_t system) {

    system_t result = rk4System(H, system);

    putPivotPositionSystem(&result);
    putAngleSystem(&result);
    putPendulumGravityCenterSystem(&result);
    putInertiaCenterSystem(&result);
    putPendulumVelocityVectorSystem(&result);
    putEnergySystem(&result);

    return result;
}


/*
 * Writes the value of the pendulum speed vector in the structure.
 */
void putPendulumVelocityVectorSystem(system_t *system) {

    system->pendulumVelocityVector.x = system->trolley.velocityVector.x +
                                       system->lengthPendulum * system->pivot.rotationSpeed * cos(system->pivot.angle);

    system->pendulumVelocityVector.y = system->trolley.velocityVector.y +
                                       system->lengthPendulum * system->pivot.rotationSpeed * sin(system->pivot.angle);
}


/*
 * Writes the location of the pivot in the structure.
 */
void putPivotPositionSystem(system_t *system) {

    system->pivot.position.x = system->trolley.position.x;
    system->pivot.position.y = system->trolley.position.y;
}


/*
 * Returns the potential energy of the system.
 */
double potentialEnergySystem(system_t system) {

    return (system.weight.mass + system.bar.mass) * GRAVITY * system.lengthPendulum *
           (1.0 - cos(system.pivot.angle));
}


/*
 * Returns the kinetic energy of the system.
 */
double kineticEnergySystem(system_t system) {

    return system.trolley.mass * pow(vectorNorm(system.trolley.velocityVector), 2.0) / 2.0 +
           (system.weight.mass + system.bar.mass) * pow(vectorNorm(system.pendulumVelocityVector), 2.0) /
           2.0;
}


/*
 * Returns the vector of the linear friction forces.
 */
point_t buildLinearFriction(double speed, double friction) {

    point_t vector;

    vector.x = speed;
    vector.y = 0.0;

    vector.x = speed * -friction;

    return vector;
}


/*
 * Returns the vector of the rotation friction forces.
 */
point_t buildRotationFriction(double angle, double length, double speedRot, double friction) {

    point_t vector;

    vector.x = length * speedRot * cos(angle);
    vector.y = length * speedRot * sin(angle);

    vector.x *= -friction;
    vector.y *= -friction;

    return vector;
}


double *getEquationCoefficients(double angle, double rotationSpeed, double trolleySpeed, system_t system) {

    double massPendulum = system.bar.mass + system.weight.mass;
    double massTrolley = system.trolley.mass;
    double length = system.lengthPendulum;


    point_t rotFriction = buildRotationFriction(angle, system.lengthPendulum, rotationSpeed, system.pivot.friction);
    point_t linFriction = buildLinearFriction(trolleySpeed, system.trolley.friction);


    double *result = malloc(sizeof(double) * 2);
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    result[0] = massPendulum * length * sin(angle) * pow(rotationSpeed, 2.0) +
                cos(angle) * sin(angle) * (massPendulum * GRAVITY - rotFriction.y) +
                pow(sin(angle), 2.0) * rotFriction.x + linFriction.x;

    result[1] = massTrolley + massPendulum - massPendulum * pow(cos(angle), 2.0);

    return result;
}


/*
 * Differential equation for pivot rotation speed evolution.
 */
double rotationSpeedEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    point_t rotFriction = buildRotationFriction(angle, system.lengthPendulum, rotationSpeed, system.pivot.friction);

    double *cc = getEquationCoefficients(angle, rotationSpeed, trolleySpeed, system);

    double result = -GRAVITY / system.lengthPendulum * sin(angle) - cos(angle) / system.lengthPendulum * cc[0] / cc[1] +
                    sin(angle) / (system.bar.mass + system.weight.mass) / system.lengthPendulum * rotFriction.y +
                    cos(angle) / (system.bar.mass + system.weight.mass) / system.lengthPendulum * rotFriction.x;

    free(cc);

    return result;
}


/*
 * Differential equations for trolley speed evolution.
 */
double linearSpeedEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    double *cc = getEquationCoefficients(angle, rotationSpeed, trolleySpeed, system);

    double result = cc[0] / cc[1];

    free(cc);

    return result;
}


/*
 * Puts the system in a state without energy.
 */
void stopSystem(system_t *system) {

    system->pivot.angle = 0.0;
    system->pivot.rotationSpeed = 0.0;
    system->trolley.velocityVector.x = 0.0;
    system->trolley.velocityVector.y = 0.0;

    *system = nextTimeSystem(*system);
}


/*
 * Solve the differential equations at time t + h with the rk4 algorithm.
 * And return the system to the corresponding time.
 * Only the variables of the equations are modified (position, angle, speed).
 */
system_t rk4System(double h, system_t system) {

    system_t systemH = system;

    double trolleySpeed = system.trolley.velocityVector.x;
    double angle = system.pivot.angle;
    double rotationSpeed = system.pivot.rotationSpeed;

    double trolleyPosition_k[4];
    double rotationPosition_k[4];
    double trolleySpeed_k[4];
    double rotationSpeed_k[4];

    trolleyPosition_k[0] = h * trolleySpeed;
    trolleySpeed_k[0] = h * linearSpeedEquation(angle, trolleySpeed, rotationSpeed, system);
    rotationPosition_k[0] = h * rotationSpeed;
    rotationSpeed_k[0] = h * rotationSpeedEquation(angle, trolleySpeed, rotationSpeed, system);

    trolleyPosition_k[1] = h * (trolleySpeed + trolleySpeed_k[0] / 2);
    trolleySpeed_k[1] =
            h * linearSpeedEquation(angle + rotationPosition_k[0] / 2, trolleySpeed + trolleySpeed_k[0] / 2,
                                    rotationSpeed + rotationSpeed_k[0] / 2, system);
    rotationPosition_k[1] = h * (rotationSpeed + rotationSpeed_k[0] / 2);
    rotationSpeed_k[1] =
            h * rotationSpeedEquation(angle + rotationPosition_k[0] / 2, trolleySpeed + trolleySpeed_k[0] / 2,
                                      rotationSpeed + rotationSpeed_k[0] / 2, system);

    trolleyPosition_k[2] = h * (trolleySpeed + trolleySpeed_k[1] / 2);
    trolleySpeed_k[2] =
            h * linearSpeedEquation(angle + rotationPosition_k[1] / 2, trolleySpeed + trolleySpeed_k[1] / 2,
                                    rotationSpeed + rotationSpeed_k[1] / 2, system);
    rotationPosition_k[2] = h * (rotationSpeed + rotationSpeed_k[1] / 2);
    rotationSpeed_k[2] =
            h * rotationSpeedEquation(angle + rotationPosition_k[1] / 2, trolleySpeed + trolleySpeed_k[1] / 2,
                                      rotationSpeed + rotationSpeed_k[1] / 2, system);

    trolleyPosition_k[3] = h * (trolleySpeed + trolleySpeed_k[2]);
    trolleySpeed_k[3] = h * linearSpeedEquation(angle + rotationPosition_k[2], trolleySpeed + trolleySpeed_k[2],
                                                rotationSpeed + rotationSpeed_k[2], system);
    rotationPosition_k[3] = h * (rotationSpeed + rotationSpeed_k[2]);
    rotationSpeed_k[3] = h * rotationSpeedEquation(angle + rotationPosition_k[2], trolleySpeed + trolleySpeed_k[2],
                                                   rotationSpeed + rotationSpeed_k[2],
                                                   system);


    systemH.trolley.position.x +=
            (trolleyPosition_k[0] + 2 * trolleyPosition_k[1] + 2 * trolleyPosition_k[2] + trolleyPosition_k[3]) / 6.0;
    systemH.pivot.angle +=
            (rotationPosition_k[0] + 2 * rotationPosition_k[1] + 2 * rotationPosition_k[2] + rotationPosition_k[3]) /
            6.0;
    systemH.trolley.velocityVector.x +=
            (trolleySpeed_k[0] + 2 * trolleySpeed_k[1] + 2 * trolleySpeed_k[2] + trolleySpeed_k[3]) / 6.0;
    systemH.pivot.rotationSpeed +=
            (rotationSpeed_k[0] + 2 * rotationSpeed_k[1] + 2 * rotationSpeed_k[2] + rotationSpeed_k[3]) / 6.0;

    return systemH;
}


// <temps>  <position>  <angle>
void printLineSystem(system_t system, double time) {

    double position = system.trolley.position.x;
    double angle = system.pivot.angle;

    printf("%lf\t%lf\t%lf\n", time, position * 100.0, angle * (180 / PI));
}


int main(int argc, char **argv) {


    if (argc != 3) {
        fprintf(stderr, "usage : pendule_22 <angle °> <potion (cm)>\n");
        exit(EXIT_FAILURE);
    }

    double iAngle = atof(argv[1]);
    double iPosition = atof(argv[2]) / 100.0;

    if (sqrt(pow(iAngle, 2.0)) > 180.0) {
        fprintf(stderr, "Angle must be between -180° and 180°\n");
        exit(EXIT_FAILURE);
    }

    // printf("Time (s)\tPosition (cm)\tAngle (°)\n");

    double time = 0.0;

    setlocale(LC_NUMERIC, "fr_FR.UTF-8");

    system_t system = buildSystem(iAngle, iPosition);

    while (system.mechanicalEnergy >= 0.000001) {

        printLineSystem(system, time);

        system = nextTimeSystem(system);
        time += H;
    }


    stopSystem(&system);

    printLineSystem(system, time);

    return 0;
}


