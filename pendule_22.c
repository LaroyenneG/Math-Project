//
// Created by guillaume on 14/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

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
#define BAR_DIAMETER 0.001
#define BAR_MASS 0.2
#define FLYWEIGHT_HEIGHT 0.007
#define OUTER_DIAMETER_FLYWEIGHT 0.006
#define DISTANCE_CENTER_FLYWEIGHT_PINTLE 0.95
#define MASS_FLYWEIGHT 0.30
#define MASS_CAR 1.20
#define LINEAR_FRICTION_CAR 0.001
#define PINTLE_FRICTION 20.0

#define H 0.03


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
    double velocityAngle;
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
    double mechanicalEnergy;
    double potentialEnergy;
    double kineticEnergy;

} system_t;


system_t buildSystem(double angle);


double potentialEnergySystem(system_t system);

double kineticEnergySystem(system_t system);

point_t pendulumGravityCenterSystem(system_t system);

point_t interpolatePoint(point_t point1, point_t point2, double f);

point_t rectangleGravityCenter(point_t a, point_t b);


num_t rk4(num_t (*f)(num_t, num_t), num_t h, num_t x, num_t y);

num_t euler(num_t (*f)(num_t, num_t), num_t t0, num_t tf, num_t y0, int app);

/****************************************************************************************************
 * end pendule_22.h
 ***************************************************************************************************/


point_t interpolatePoint(point_t point1, point_t point2, double f) {

    point_t result;

    result.x = f * point1.x + (1 - f) * point2.x;
    result.y = f * point1.y + (1 - f) * point2.y;

    return result;
}

double distancePoint(point_t point1, point_t point2) {

    return sqrt(pow(point1.x - point2.x, 2.0) + pow(point2.y - point2.y, 2.0));
}

// angle en degré !
system_t buildSystem(double angle) {

    system_t system;

    system.pivot.angle = angle * PI / 180;
    system.pivot.velocityAngle = 0.0;

    system.trolley.position.x = 0.0;
    system.trolley.position.y = 0.0;
    system.trolley.mass = MASS_CAR;
    system.trolley.friction = 0.0;
    system.trolley.velocityVector.x = 0.0;
    system.trolley.velocityVector.y = 0.0;

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
    system.mechanicalEnergy = system.mechanicalEnergy + system.kineticEnergy;


    system.pendulumGravityCenter = pendulumGravityCenterSystem(system);

    system.inertiaCenter = interpolatePoint(system.trolley.position, system.weight.gravityCenter,
                                            system.trolley.mass / (system.trolley.mass + system.weight.mass));

    return system;
}

point_t pendulumGravityCenterSystem(system_t system) {

    point_t g;

    g.x = (system.bar.gravityCenter.x + system.weight.gravityCenter.x) / (system.bar.mass + system.weight.mass);
    g.y = (system.bar.gravityCenter.y + system.weight.gravityCenter.y) / (system.bar.mass + system.weight.mass);

    return g;
}


point_t rectangleGravityCenter(point_t a, point_t b) {

    point_t g;

    g.x = (b.x - a.x) / 2;
    g.y = (b.y - a.y) / 2;

    return g;
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

    setlocale(LC_NUMERIC, "fr_FR.UTF-8");

    system_t system = buildSystem(10.0);

    return 0;
}
