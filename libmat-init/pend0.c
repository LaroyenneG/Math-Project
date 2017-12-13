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


    mat_t *mat = mat_new(4, 4);
    mat = mat_rnd(mat, 5);

    mat_print(mat, "Matrice aléatoire");


    printf("Det(A)=%lf\n", mat_det(mat));

    mat_print(mat_red(mat, 1,1, NULL),"Matrice réduite en 1, 1");

}
