#include <stdlib.h>
#include <stdio.h>

#include "libdebug.h"

int main(int argc, char *argv[]) {
	int i;
	for (i=0;i<5;i++) {
		debug("i> %d",i);
		trace();
		printf("printf : i> %d\n",i);
	}
}
