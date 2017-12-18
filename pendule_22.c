//
// Created by guillaume on 14/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

#include "libmat.h"

#define H 0.1


num_t rk4(num_t (*f)(num_t, num_t), num_t h, num_t x, num_t y);

num_t euler(num_t (*f)(num_t, num_t), num_t t0, num_t tf, num_t y0, int app);

num_t f(num_t t, num_t y) {
    return 1 - y;
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


    num_t y = 0.0;
    num_t t = 0.0;
    num_t h = H;


    printf("t\t\t1-exp(-t)\ty (rk4)\n");
    for (int i = 0; i < 10; ++i, t += h) {
        printf("%lf\t%lf\t%lf\n", t, 1 - exp(-t), y);
        y = rk4(&f, h, 0.0, y);
    }


    num_t ** tt = build_euler(&f,0.0, 20.0,0.0, 100);

    for (int j = 0; j < 100; ++j) {
        printf("%lf\n",tt[0][j]);
    }

    return 0;
}
