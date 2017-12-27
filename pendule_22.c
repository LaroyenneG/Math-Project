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

static point_t origin = {0.0, 0.0};

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

    return sqrt(pow(v.x, 2.0) + pow(v.x, 2.0));
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
    putPivotSystem(&system);


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


    system.pendulumGravityCenter = pendulumGravityCenterSystem(system);
    system.inertiaCenter = interpolatePoint(system.trolley.position, system.weight.gravityCenter,
                                            system.trolley.mass / (system.trolley.mass + system.weight.mass));

    system.lengthBar = distancePoint(system.pivot.position, system.bar.gravityCenter);
    system.lengthWeight = distancePoint(system.pivot.position, system.weight.gravityCenter);
    system.lengthPendulum = distancePoint(system.pivot.position, system.pendulumGravityCenter);



    system.pendulumVelocityVector.x = system.trolley.velocityVector.x +
                                      system.lengthPendulum * system.pivot.rotationSpeed * cos(system.pivot.angle);
    system.pendulumVelocityVector.y = system.trolley.velocityVector.y +
                                      system.lengthPendulum * system.pivot.rotationSpeed * sin(system.pivot.angle);





    applyAngleSystem(&system);


    computeEnergySystem(&system);

    return system;
}


void computeEnergySystem(system_t *system) {
    system->kineticEnergy = kineticEnergySystem(*system);
    system->potentialEnergy = potentialEnergySystem(*system);
    system->mechanicalEnergy = system->potentialEnergy + system->kineticEnergy;
}


point_t pendulumGravityCenterSystem(system_t system) {

    point_t g;

    g.x = (system.bar.gravityCenter.x * system.bar.mass + system.weight.gravityCenter.x * system.weight.mass) /
          (system.bar.mass + system.weight.mass);

    g.y = (system.bar.gravityCenter.y * system.bar.mass + system.weight.gravityCenter.y * system.weight.mass) /
          (system.bar.mass + system.weight.mass);

    return g;
}


point_t rectangleGravityCenter(point_t a, point_t b) {

    point_t g;

    g.x = (b.x + a.x) / 2.0;
    g.y = (b.y + a.y) / 2.0;

    return g;
}

void applyAngleSystem(system_t *system) {

    system->weight.gravityCenter.x = system->pivot.position.x + system->lengthWeight * sin(system->pivot.angle);
    system->weight.gravityCenter.y = system->pivot.position.y + system->lengthWeight * cos(system->pivot.angle);

    system->bar.gravityCenter.x = system->pivot.position.x + system->lengthBar * sin(system->pivot.angle);
    system->bar.gravityCenter.y = system->pivot.position.y + system->lengthBar * cos(system->pivot.angle);
}


system_t nextTimeSystem(system_t system) {

    system_t result = rk4System(H, system);
    putPivotSystem(&result);

    applyAngleSystem(&result);

    computeEnergySystem(&result);

    return result;
}


void putPivotSystem(system_t *system) {

    system->pivot.position.x = system->trolley.position.x;
    system->pivot.position.y = system->trolley.position.y;
}


// <temps>  <position>  <angle>
void printLineSystem(system_t system, double time) {

    double position = system.trolley.position.x;
    double angle = system.pivot.angle;

    printf("%lf\t%lf\t\t%lf", time, position, angle);
}


double potentialEnergySystem(system_t system) {
    return (system.weight.mass + system.bar.mass) * GRAVITY * system.lengthPendulum *
           (1.0 - cos(system.pivot.angle));
}

double kineticEnergySystem(system_t system) {

    return 0.5 * (system.trolley.mass * pow(distancePoint(system.pendulumVelocityVector, origin), 2.0) +
                  0.5 * (system.weight.mass + system.bar.mass) *
                  pow(distancePoint(system.pendulumVelocityVector, origin), 2.0));
}


double applyLinearFriction(double speed, double friction) {

    return speed * (-friction * speed);
}


double aang(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    double x = cos(angle);
    double y = sin(angle);

    double massPendulum = system.bar.mass + system.weight.mass;
    double massTrolley = system.trolley.mass;
    double length = system.lengthPendulum;

    point_t vectorPendulum;
    vectorPendulum.x = trolleySpeed + length * rotationSpeed * cos(angle);
    vectorPendulum.y = length * rotationSpeed * sin(angle);


    double a =
            massPendulum * length * y * pow(rotationSpeed, 2.0) + x * y * (massPendulum * GRAVITY - vectorPendulum.y) +
            pow(y, 2.0) * vectorPendulum.x +
            applyLinearFriction(trolleySpeed, system.trolley.friction);
    double b = massTrolley + massPendulum - massPendulum * pow(x, 2.0);

    return -GRAVITY / length * y - x / length * a / b + y / massPendulum / length * vectorPendulum.y +
           x / massPendulum / length * vectorPendulum.x;

}


double alin(double angle, double trolleySpeed, double rotationSpeed, system_t system) {

    double x = cos(angle);
    double y = sin(angle);

    double massPendulum = system.bar.mass + system.weight.mass;
    double massTrolley = system.trolley.mass;
    double length = system.lengthPendulum;

    point_t vectorPendulum;
    vectorPendulum.x = trolleySpeed + length * rotationSpeed * cos(angle);
    vectorPendulum.y = length * rotationSpeed * sin(angle);

    double a =
            massPendulum * length * y * pow(rotationSpeed, 2.0) + x * y * (massPendulum * GRAVITY - vectorPendulum.y) +
            pow(y, 2.0) * vectorPendulum.x +
            applyLinearFriction(trolleySpeed, system.trolley.friction);
    double b = massTrolley + massPendulum - massPendulum * pow(x, 2.0);

    return a / b;
}

system_t rk4System(double h, system_t system) {

    system_t systemH = system;

    double trolleySpeed = system.trolley.velocityVector.x;
    double angle = system.pivot.angle;
    double rotationSpeed = system.pivot.rotationSpeed;

    double k1a = h * trolleySpeed;
    double k1c = h * alin(angle, trolleySpeed, rotationSpeed, system);
    double k1b = h * rotationSpeed;
    double k1d = h * aang(angle, trolleySpeed, rotationSpeed, system);
    double k2a = h * (trolleySpeed + k1c / 2);
    double k2c = h * alin(angle + k1b / 2, trolleySpeed + k1c / 2, rotationSpeed + k1d / 2, system);
    double k2b = h * (rotationSpeed + k1d / 2);
    double k2d = h * aang(angle + k1b / 2, trolleySpeed + k1c / 2, rotationSpeed + k1d / 2, system);
    double k3a = h * (trolleySpeed + k2c / 2);
    double k3c = h * alin(angle + k2b / 2, trolleySpeed + k2c / 2, rotationSpeed + k2d / 2, system);
    double k3b = h * (rotationSpeed + k2d / 2);
    double k3d = h * aang(angle + k2b / 2, trolleySpeed + k2c / 2, rotationSpeed + k2d / 2, system);
    double k4a = h * (trolleySpeed + k3c);
    double k4c = h * alin(angle + k3b, trolleySpeed + k3c, rotationSpeed + k3d, system);
    double k4b = h * (rotationSpeed + k3d);
    double k4d = h * aang(angle + k3b, trolleySpeed + k3c, rotationSpeed + k3d, system);

    double ka = (k1a + 2 * k2a + 2 * k3a + k4a) / 6;
    double kb = (k1b + 2 * k2b + 2 * k3b + k4b) / 6;
    double kc = (k1c + 2 * k2c + 2 * k3c + k4c) / 6;
    double kd = (k1d + 2 * k2d + 2 * k3d + k4d) / 6;

    systemH.trolley.position.x += ka;
    systemH.pivot.angle += kb;
    systemH.trolley.velocityVector.x += kc;
    systemH.pivot.rotationSpeed += kd;

    return systemH;
}


int main(int argc, char **argv) {

    double time = 0.0;


    setlocale(LC_NUMERIC, "fr_FR.UTF-8");

    // showSystemZero();

    system_t system = buildSystem(70.0, 0.0);


    while (system.mechanicalEnergy >= 0.0) {
        showSystemTime(system);
        system = nextTimeSystem(system);
        time += H;
    }


    return 0;
}


