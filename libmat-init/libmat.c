#include <stdlib.h>

#include "libmat.h"


int my_pow(int a, int p) {

    int r = 1;

    for (int i = 0; i < p; ++i) {
        r *= a;
    }

    return r;
}

mat_t *mat_add(mat_t *m1, mat_t *m2, mat_t *r) {
    return NULL;
}

num_t mat_det(mat_t *m) {

    if (m->nl != m->nc) {
        fprintf(stderr, "Error matrix size");
        return 0;
    }

    if (m->nc == 2 && m->nl == 2) {
        return m->m[0] * m->m[3] - m->m[2] * m->m[1];
    }

    num_t det = 0;

    for (int l = 0; l < m->nc; ++l) {
        int signe = my_pow(-1, l + 2);


        mat_t *sub_mat = mat_new(m->nc - 1, m->nl - 1);

        int count = 0;
        for (int i = 1; i < m->nl; ++i) {
            for (int j = 0; j < m->nc; ++j) {
                if (j != l) {
                    sub_mat->m[count] = mat_get(m, i, j);
                    count++;
                }
            }
        }

        num_t value = mat_get(m, 0, l);
        num_t detSubMat = mat_det(sub_mat);

        det += signe * value * detSubMat;

        mat_free(sub_mat);
    }

    return det;
}

void mat_free(mat_t *m) {
    free(m->m);
    free(m);
}

num_t mat_get(mat_t *m, int i, int j) {
    return m->m[i * m->nc + j];
}

mat_t *mat_inv(mat_t *m, mat_t *r) {
    return NULL;
}

mat_t *mat_new(int i, int j) {
    mat_t *m = (mat_t *) malloc(sizeof(mat_t));
    if (m == NULL) return NULL;
    m->nl = i;
    m->nc = j;
    m->m = (num_t *) malloc(i * j * sizeof(num_t));
    if (m == NULL) return NULL;
    return m;
}

void mat_print(mat_t *m, char *s) {
    int i, j;
    if (s != NULL) {
        printf("%s\n", s);
    }
    for (j = 0; j < m->nc; j++) printf("+------------");
    printf("+\n");
    for (i = 0; i < m->nl; i++) {
        for (j = 0; j < m->nc; j++) {
            printf("| %10.3g ", m->m[i * m->nc + j]);
        }
        printf("|\n");
    }
    for (j = 0; j < m->nc; j++) printf("+------------");
    printf("+\n");
}

mat_t *mat_prod(mat_t *m1, mat_t *m2, mat_t *r) {
    /*
     * Si r (résultat est NULL), crée une nouvelle matrice
     */
    return NULL;
}

mat_t *mat_raz(mat_t *m) {
    int i, j;
    for (i = 0; i < m->nl; i++) {
        for (j = 0; j < m->nc; j++) {
            m->m[i * m->nc + j] = 0.0;
        }
    }
    return m;
}

mat_t *mat_red(mat_t *m, int l, int c, mat_t *r) {
    /* renvoie la matrice carrée réduite après suppression
     * de la ligne l et de la colonne c */
    return NULL;
}

mat_t *mat_rnd(mat_t *m, num_t vmax) {
    int i, j;
    for (i = 0; i < m->nl; i++) {
        for (j = 0; j < m->nc; j++) {
            m->m[i * m->nc + j] = vmax * rand() / (num_t) RAND_MAX;
        }
    }
    return m;
}

void mat_set(mat_t *m, int i, int j, num_t v) {
    m->m[i * m->nc + j] = v;
}

mat_t *mat_sub(mat_t *m1, mat_t *m2, mat_t *r) {
    return NULL;
}

mat_t *mat_transp(mat_t *m, mat_t *r) {
    return NULL;
}

mat_t *mat_unity(mat_t *m) {
    int i, j;
    for (i = 0; i < m->nl; i++) {
        for (j = 0; j < m->nc; j++) {
            if (i == j) m->m[i * m->nc + j] = 1.0;
            else m->m[i * m->nc + j] = 0.0;
        }
    }
    return m;
}

