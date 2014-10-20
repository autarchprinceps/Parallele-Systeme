/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20.10.2014 11:23:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void work() {
	int i;
	#pragma omp for
	for(i = 0; i < 36; i++) {
		printf("%i %i\n", i, omp_get_thread_num());
	}
	printf("[DEBUG]: End work\n");
}

int main() {
	#pragma omp parallel
	{
		work();
		printf("[DEBUG]: After work %i\n", omp_get_thread_num());
	}
	printf("[DEBUG]: End main\n");
}
