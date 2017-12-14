#include <stdlib.h>
#include <stdio.h>

#include "libdebug.h"
#include "libmat.h"

int main(int argc, char *argv[]) {

    mat_t *mat1 = mat_new(3, 3);
    mat1 = mat_rnd(mat1, 3);

    mat_print(mat1, "Matrice M1");

    mat_t *inv = mat_inv(mat1, NULL);

    mat_print(inv, "Matrice M1^-1");

    mat_free(mat1);
    mat_free(inv);


    return 0;
}
