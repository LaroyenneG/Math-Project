#include <stdlib.h>

#include "libmat.h"


int my_pow(int a, int p) {

    int r = 1;

    for (int i = 0; i < p; ++i, r *= a);

    return r;
}

mat_t *mat_add(mat_t *m1, mat_t *m2, mat_t *r) {

    if (m1->nc != m2->nc || m1->nl != m2->nl) {
        return NULL;
    }

    if (r == NULL) {
        r = mat_new(m1->nl, m1->nc);
    } else if (r->nc != m1->nc || r->nl != m1->nl) {
        return NULL;
    }

    for (int i = 0; i < r->nl; ++i) {
        for (int j = 0; j < r->nc; ++j) {
            num_t v = mat_get(m1, i, j) + mat_get(m2, i, j);
            mat_set(r, i, j, v);
        }
    }

    return r;
}


mat_t *mat_cof(mat_t *m, mat_t *r) {

    if (m->nc != m->nl) {
        return NULL;
    }

    if (r == NULL) {
        r = mat_new(m->nl, m->nc);
    } else if (r->nc != m->nc || r->nl != m->nl) {
        return NULL;
    }

    for (int i = 0; i < m->nl; ++i) {
        for (int j = 0; j < m->nc; ++j) {

            mat_t *sub_mat = mat_red(m, i, j, NULL);
            int sine = my_pow(-1, i + j + 2);
            num_t v = sine * mat_det(sub_mat);
            mat_set(r, i, j, v);

            mat_free(sub_mat);
        }
    }

    return r;
}

num_t mat_det(mat_t *m) {

    if (m->nl != m->nc) {
        fprintf(stderr, "mat_det() : invalid matrix dimension\n");
        return 0;
    }

    if (m->nc == 2 && m->nl == 2) {
        return m->m[0] * m->m[3] - m->m[2] * m->m[1];
    }

    num_t det = 0;

    for (int l = 0; l < m->nc; ++l) {
        int sine = my_pow(-1, l + 2);

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

        det += sine * value * detSubMat;

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

mat_t *mat_val(mat_t *m, num_t v) {
    for (int i = 0; i < m->nl; ++i) {
        for (int j = 0; j < m->nc; ++j) {
            if (j == i) {
                mat_set(m, i, j, v);
            } else {
                mat_set(m, i, j, 0.0);
            }
        }
    }

    return m;
}

mat_t *mat_inv(mat_t *m, mat_t *r) {

    if (m->nl != m->nc) {
        return NULL;
    }

    if (r == NULL) {
        r = mat_new(m->nl, m->nc);
    } else if (r->nc != m->nc || r->nl != m->nl) {
        return NULL;
    }

    num_t detM = mat_det(m);

    if (detM == 0) {
        return NULL;
    }

    mat_t *comM = mat_cof(m, NULL);
    mat_t *transComM = mat_transp(comM, NULL);

    mat_t *matVal = mat_val(mat_new(m->nl, m->nc), 1.0 / detM);

    mat_prod(matVal, transComM, r);

    mat_free(comM);
    mat_free(transComM);
    mat_free(matVal);

    return r;
}

mat_t *mat_new(int i, int j) {

    mat_t *m = (mat_t *) malloc(sizeof(mat_t));
    if (m == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    m->nl = i;
    m->nc = j;

    m->m = (num_t *) malloc(i * j * sizeof(num_t));
    if (m->m == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

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

    if (m1->nc != m2->nl) {
        return NULL;
    }

    int len = m1->nc;

    if (r == NULL) {
        r = mat_new(m1->nl, m2->nc);
    } else if (r->nc != m2->nc || r->nl != m1->nl) {
        return NULL;
    }

    for (int i = 0; i < m1->nl; ++i) {
        for (int j = 0; j < m2->nc; ++j) {
            int v = 0;

            for (int k = 0; k < len; ++k) {
                num_t a = mat_get(m1, i, k);
                num_t b = mat_get(m2, k, j);
                v += a * b;
            }

            mat_set(r, i, j, v);
        }
    }

    return r;
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

    if (m->nc != m->nl) {
        return NULL;
    }


    if (r == NULL) {
        r = mat_new(m->nl - 1, m->nc - 1);
    } else if (r->nl != m->nl - 1 || r->nc != m->nc - 1) {
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < m->nl; ++i) {
        if (i != l) {
            for (int j = 0; j < m->nc; ++j) {
                if (j != c) {
                    r->m[count] = mat_get(m, i, j);
                    count++;
                }
            }
        }
    }

    return r;
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
    if (m1->nc != m2->nc || m1->nl != m2->nl) {
        return NULL;
    }

    if (r == NULL) {
        r = mat_new(m1->nl, m1->nc);
    } else if (r->nc != m1->nc || r->nl != m1->nl) {
        return NULL;
    }

    for (int i = 0; i < r->nl; ++i) {
        for (int j = 0; j < r->nc; ++j) {
            num_t v = mat_get(m1, i, j) - mat_get(m2, i, j);
            mat_set(r, i, j, v);
        }
    }

    return r;
}

mat_t *mat_transp(mat_t *m, mat_t *r) {

    if (r == NULL) {
        r = mat_new(m->nc, m->nl);
    } else if (r->nc != m->nl || r->nl != m->nc) {
        return NULL;
    }

    for (int i = 0; i < m->nl; ++i) {
        for (int j = 0; j < m->nc; ++j) {
            mat_set(r, j, i, mat_get(m, i, j));
        }
    }

    return r;
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

