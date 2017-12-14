#ifndef _LIB_MAT_H
#define _LIB_MAT_H

/*
 * +"design system for linear multivariable control" Armstrong pdf 
 *
 */

#include <stdio.h>

typedef double num_t;

typedef struct {
    int nl, nc;
    num_t *m;
} mat_t;

mat_t *mat_add(mat_t *m1, mat_t *m2, mat_t *r);

num_t mat_det(mat_t *m);

void mat_free(mat_t *m);

num_t mat_get(mat_t *m, int i, int j);

mat_t *mat_inv(mat_t *m1, mat_t *r);

mat_t *mat_new(int i, int j);

void mat_print(mat_t *m, char *s);

mat_t *mat_prod(mat_t *m1, mat_t *m2, mat_t *resu);

mat_t *mat_raz(mat_t *m);

mat_t *mat_red(mat_t *m, int l, int c, mat_t *r);

mat_t *mat_rnd(mat_t *m, num_t vmax);

void mat_set(mat_t *m, int i, int j, num_t v);

mat_t *mat_sub(mat_t *m1, mat_t *m2, mat_t *r);

mat_t *mat_transp(mat_t *m, mat_t *r);

mat_t *mat_unity(mat_t *m);

mat_t *mat_cof(mat_t *m, mat_t *r);

mat_t *mat_val(mat_t *m, num_t v);

int my_pow(int a, int p);

#endif