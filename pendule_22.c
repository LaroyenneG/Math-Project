//
// Created by guillaume on 14/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <zconf.h>

#include "libmat.h"
#include "delete-me.h"
#include "view.h"


point_t interpolatePoint(point_t point1, point_t point2, double f) {

    point_t result;

    result.x = f * point1.x + (1 - f) * point2.x;
    result.y = f * point1.y + (1 - f) * point2.y;

    return result;
}

double distancePoint(point_t point1, point_t point2) {

    return sqrt(pow(point1.x - point2.x, 2.0) + pow(point1.y - point2.y, 2.0));
}

double vectorNorm(point_t v) {

    return sqrt(pow(v.x, 2.0) + pow(v.y, 2.0));
}

// angle en degré !
system_t buildSystem(double angle, double p0) {

    system_t system;

    system.trolley.position.x = p0;
    system.trolley.position.y = 0.0;
    system.trolley.mass = MASS_CAR;
    system.trolley.friction = 0.0;
    system.trolley.velocityVector.x = 0.0;
    system.trolley.velocityVector.y = 0.0;


    system.pivot.angle = angle * PI / 180;
    system.pivot.rotationSpeed = 0.0;
    system.pivot.friction = 0.0;
    putPivotPositionSystem(&system);


    system.weight.mass = MASS_FLYWEIGHT;
    system.weight.velocityVector.x = 0.0;
    system.weight.velocityVector.y = 0.0;
    system.weight.a.x = system.pivot.position.x - OUTER_DIAMETER_FLYWEIGHT / 2.0;
    system.weight.a.y = system.pivot.position.y + (DISTANCE_CENTER_FLYWEIGHT_PINTLE - FLYWEIGHT_HEIGHT / 2.0);
    system.weight.b.x = system.pivot.position.x + OUTER_DIAMETER_FLYWEIGHT / 2.0;
    system.weight.b.y = system.pivot.position.y + (DISTANCE_CENTER_FLYWEIGHT_PINTLE + FLYWEIGHT_HEIGHT / 2.0);
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


void putEnergySystem(system_t *system) {

    system->kineticEnergy = kineticEnergySystem(*system);
    system->potentialEnergy = potentialEnergySystem(*system);
    system->mechanicalEnergy = system->potentialEnergy + system->kineticEnergy;
}


/*
 *
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
 * Returns the center of gravity of a rectangle.
 */
point_t rectangleGravityCenter(point_t a, point_t b) {

    point_t g;

    g.x = (b.x + a.x) / 2.0;
    g.y = (b.y + a.y) / 2.0;

    return g;
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
 * Returns the values of the linear friction forces.
 */
point_t applyLinearFriction(double speed, double friction) {

    point_t vector;

    vector.x = speed;
    vector.y = 0.0;

    // application des frottements

    point_t cpVector = vector;

    vector.x *= (-friction * vectorNorm(cpVector));
    vector.y *= (-friction * vectorNorm(cpVector));

    return vector;
}


/*
 * Returns the values of the rotation friction forces.
 */
point_t applyRotationFriction(double angle, double length, double speedRot, double speedTro, double friction) {


    point_t vector;

    vector.x = speedTro + length * speedRot * cos(angle);
    vector.y = length * speedRot * sin(angle);

    // application des frottements

    point_t cpVector = vector;

    vector.x *= (-friction * vectorNorm(cpVector));
    vector.y *= (-friction * vectorNorm(cpVector));

    return vector;
}


double angleEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    double x = cos(angle);
    double y = sin(angle);

    double massPendulum = system.bar.mass + system.weight.mass;
    double massTrolley = system.trolley.mass;
    double length = system.lengthPendulum;


    point_t vectorRot = applyRotationFriction(angle, system.lengthPendulum, rotationSpeed, trolleySpeed, 0.0);

    double a =
            massPendulum * length * y * pow(rotationSpeed, 2.0) + x * y * (massPendulum * GRAVITY - vectorRot.y) +
            pow(y, 2.0) * vectorRot.x +
            applyLinearFriction(trolleySpeed, system.trolley.friction).x;

    double b = massTrolley + massPendulum - massPendulum * pow(x, 2.0);

    return -GRAVITY / length * y - x / length * a / b + y / massPendulum / length * vectorRot.y +
           x / massPendulum / length * vectorRot.x;

}


double linearEquation(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    double x = cos(angle);
    double y = sin(angle);

    double massPendulum = system.bar.mass + system.weight.mass;
    double massTrolley = system.trolley.mass;
    double length = system.lengthPendulum;

    point_t vectorRot = applyRotationFriction(angle, system.lengthPendulum, rotationSpeed, trolleySpeed, 0.0);

    double a =
            massPendulum * length * y * pow(rotationSpeed, 2.0) + x * y * (massPendulum * GRAVITY - vectorRot.y) +
            pow(y, 2.0) * vectorRot.x +
            applyLinearFriction(trolleySpeed, system.trolley.friction).x;

    double b = massTrolley + massPendulum - massPendulum * pow(x, 2.0);

    return a / b;
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

    double k1a = h * trolleySpeed;
    double k1c = h * linearEquation(angle, trolleySpeed, rotationSpeed, system);
    double k1b = h * rotationSpeed;
    double k1d = h * angleEquation(angle, trolleySpeed, rotationSpeed, system);
    double k2a = h * (trolleySpeed + k1c / 2);
    double k2c = h * linearEquation(angle + k1b / 2, trolleySpeed + k1c / 2, rotationSpeed + k1d / 2, system);
    double k2b = h * (rotationSpeed + k1d / 2);
    double k2d = h * angleEquation(angle + k1b / 2, trolleySpeed + k1c / 2, rotationSpeed + k1d / 2, system);
    double k3a = h * (trolleySpeed + k2c / 2);
    double k3c = h * linearEquation(angle + k2b / 2, trolleySpeed + k2c / 2, rotationSpeed + k2d / 2, system);
    double k3b = h * (rotationSpeed + k2d / 2);
    double k3d = h * angleEquation(angle + k2b / 2, trolleySpeed + k2c / 2, rotationSpeed + k2d / 2, system);
    double k4a = h * (trolleySpeed + k3c);
    double k4c = h * linearEquation(angle + k3b, trolleySpeed + k3c, rotationSpeed + k3d, system);
    double k4b = h * (rotationSpeed + k3d);
    double k4d = h * angleEquation(angle + k3b, trolleySpeed + k3c, rotationSpeed + k3d, system);

    double ka = (k1a + 2 * k2a + 2 * k3a + k4a) / 6.0;
    double kb = (k1b + 2 * k2b + 2 * k3b + k4b) / 6.0;
    double kc = (k1c + 2 * k2c + 2 * k3c + k4c) / 6.0;
    double kd = (k1d + 2 * k2d + 2 * k3d + k4d) / 6.0;

    systemH.trolley.position.x += ka;
    systemH.pivot.angle += kb;
    systemH.trolley.velocityVector.x += kc;
    systemH.pivot.rotationSpeed += kd;

    return systemH;
}


// <temps>  <position>  <angle>
void printLineSystem(system_t system, double time) {

    double position = system.trolley.position.x;
    double angle = system.pivot.angle;

    printf("%lf\t%lf\t%lf\n", time, position, angle);
}


int main(int argc, char **argv) {


    if (argc > 1) {
        fprintf(stderr, "Usage : pendule_22\n");
        exit(EXIT_FAILURE);
    }


    double time = 0.0;

    setlocale(LC_NUMERIC, "fr_FR.UTF-8");

    showSystemZero();

    system_t system = buildSystem(-70.0, 0.0);

    system.trolley.friction = 0.0;

    showSystemTime(system);

    while (system.mechanicalEnergy > 0.0001) {

        showSystemTime(system);
        printLineSystem(system, time);

        system = nextTimeSystem(system);
        time += H;
    }


    return 0;
}


