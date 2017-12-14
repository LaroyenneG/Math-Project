#include <stdlib.h>
#include <stdio.h>

#include "libdebug.h"
#include "libmat.h"

int main(int argc, char *argv[]) {
    /*
    int i;
    for (i=0;i<5;i++) {
        debug("i> %d",i);
        trace();
        printf("printf : i> %d\n",i);
    }
     */

    mat_t *mat1 = mat_new(3, 3);
    mat1 = mat_rnd(mat1, 10);

    mat_print(mat1, "Matrice aléatoire 1");

    mat_t *mat2 = mat_new(3, 3);
    mat1 = mat_rnd(mat2, 10);

    mat_print(mat2, "Matrice aléatoire 2");

    mat_print(mat_prod(mat1, mat2, NULL),"M1 x M2");

    mat_free(mat1);

    return 0;
}
