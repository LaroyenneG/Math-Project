#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define FLOAT_SIZE 4

union value {
	float f;
	unsigned char c[FLOAT_SIZE];
};

int main(int argc, char *argv[]) {
	union value v;
	int i;

	assert(sizeof(float)==FLOAT_SIZE);
	printf("Taille d'un flottant (float) = %lu\n", sizeof(float));
	while (1) {
		printf("Flottant : ");
		scanf("%f",&v.f);
		for (i=0;i<sizeof(float);i++) printf("%02x ",v.c[i]);
		printf("\n");
	}
	return(EXIT_SUCCESS);
}