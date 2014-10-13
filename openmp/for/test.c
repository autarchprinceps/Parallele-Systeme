#include <omp.h>
#include <stdio.h>

int main() {
	int i;
	#pragma omp parallel for
	for(i = 0; i < 10; i++) {
		printf("loop %i\n", i);
	}
	printf("Ende\n");
}
