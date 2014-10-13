#include <omp.h>
#include <stdio.h>

int main() {
	#pragma omp parallel
	printf("Hallo Welt\n");
}
