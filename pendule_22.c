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
    system.pivot.velocityAngle = 0.0;
    system.pivot.position.x = system.trolley.position.x;
    system.pivot.position.y = system.trolley.position.y;


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


    system.kineticEnergy = kineticEnergySystem(system);
    system.potentialEnergy = potentialEnergySystem(system);
    system.mechanicalEnergy = system.potentialEnergy + system.kineticEnergy;





    // incertain

    system.lengthBar = distancePoint(system.pivot.position, system.bar.gravityCenter);
    system.lengthWeight = distancePoint(system.pivot.position, system.weight.gravityCenter);
    system.lengthPendulum = distancePoint(system.pivot.position, system.pendulumGravityCenter);


    applyAngleSystem(&system);


    system.pendulumGravityCenter = pendulumGravityCenterSystem(system);

    system.inertiaCenter = interpolatePoint(system.trolley.position, system.weight.gravityCenter,
                                            system.trolley.mass / (system.trolley.mass + system.weight.mass));

    return system;
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


    system->bar.gravityCenter.x=system->pivot.position.x+system->lengthBar*sin(system->pivot.angle);
    system->bar.gravityCenter.y=system->pivot.position.y+system->lengthBar*cos(system->pivot.angle);
}

// <temps>  <position>  <angle>
void printLineSystem(system_t system, double time) {

    double position = system.trolley.position.x;
    double angle = system.pivot.angle;

    printf("%lf\t%lf\t\t%lf", time, position, angle);
}


/*
num_t f(num_t t, num_t y) {
    return 1 - y;
}

*/

double potentialEnergySystem(system_t system) {
    return system.weight.mass * GRAVITY * distancePoint(system.pivot.position, system.weight.gravityCenter) *
           (1 - cos(system.pivot.angle));
}

double kineticEnergySystem(system_t system) {
    point_t center;
    center.x = 0.0;
    center.y = 0.0;

    return 0.5 * (system.trolley.mass * pow(distancePoint(system.trolley.velocityVector, center), 2.0) +
                  system.weight.mass * pow(distancePoint(system.weight.velocityVector, center), 2.0));
}

num_t **build_euler(num_t (*f)(num_t, num_t), num_t t0, num_t tf, num_t y0, int n) {

    num_t **table = malloc(sizeof(num_t *) * 2);
    if (table == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    num_t *y_table = malloc(sizeof(num_t) * n);
    if (y_table == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    num_t *t_table = malloc(sizeof(num_t) * n);
    if (t_table == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    num_t pas = (tf - t0) / n;
    num_t y = y0;
    num_t t = t0;

    y_table[0] = y0;
    t_table[0] = t0;

    for (int i = 1; i < n; ++i) {
        y += pas * f(t, y);
        t += pas;
        y_table[i] = y;
        t_table[i] = t;
    }

    table[0] = y_table;
    table[1] = t_table;

    return table;
}

/*
 * Retourn la valeur approché
 */
num_t euler(num_t (*f)(num_t, num_t), num_t t0, num_t tf, num_t y0, int app) {

    num_t **table = build_euler(f, t0, tf, y0, app);

    num_t s = table[0][app - 1];

    free(table[0]);
    free(table[1]);
    free(table);

    return s;
}


num_t rk4(num_t (*f)(num_t, num_t), num_t h, num_t x, num_t y) {

    num_t k1 = h * f(x, y);
    num_t k2 = h * f(x + h / 2, y + k1 / 2);
    num_t k3 = h * f(x + h / 2, y + k2 / 2);
    num_t k4 = h * f(x + h, y + k3);

    return y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}


int main(int argc, char **argv) {

    double time = 0.0;


    setlocale(LC_NUMERIC, "fr_FR.UTF-8");


    showSystemZero();


    return 0;
}
