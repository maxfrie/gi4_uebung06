#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "init_matrix.h"

#define MATRIX_SIZE 	1024

double **A;
double *b;
double *X;
double *X_old;
double *temp;

int main(int argc, char **argv)
{
	struct timeval start, end;
	unsigned int i, j;
	unsigned int iterations = 0;
	double error, xi, norm, max = 0.0;

	printf("\nInitialize system of linear equations...\n");
	/* allocate memory for the system of linear equations */
	init_matrix(&A, &b, MATRIX_SIZE);
	X = (double *)malloc(sizeof(double) * MATRIX_SIZE);
	X_old = (double *)malloc(sizeof(double) * MATRIX_SIZE);

	/* a "random" solution vector */
	for (i = 0; i < MATRIX_SIZE; i++) {
		X[i] = ((double)rand()) / ((double)RAND_MAX) * 10.0;
		X_old[i] = 0.0;
	}

	printf("Start Jacobi method...\n");

	gettimeofday(&start, NULL);

	/* Jacobi iterations */
	while (1) {
		iterations++;

		temp = X_old;
		X_old = X;
		X = temp;
		for (i = 0; i < MATRIX_SIZE; i++) {
			for (j = 0, xi = 0.0; j < i; j++)
				xi += A[i][j] * X_old[j];
			for (j = i + 1; j < MATRIX_SIZE; j++)
				xi += A[i][j] * X_old[j];
			X[i] = (b[i] - xi) / A[i][i];
		}

		if (iterations % 500 == 0) {	/* calculate the Euclidean norm between X_old and X */
			norm = 0.0;
			for (i = 0; i < MATRIX_SIZE; i++)
				norm += (X_old[i] - X[i]) * (X_old[i] - X[i]);

			/* check the break condition */
			norm /= (double)MATRIX_SIZE;
			if (norm < 0.0000001)
				break;
		}
	}

	gettimeofday(&end, NULL);

	if (MATRIX_SIZE < 16) {
		printf("Print the solution...\n");
		/* print solution */
		for (i = 0; i < MATRIX_SIZE; i++) {
			for (j = 0; j < MATRIX_SIZE; j++)
				printf("%8.2f\t", A[i][j]);
			printf("*\t%8.2f\t=\t%8.2f\n", X[i], b[i]);
		}
	}

	printf("Check the result...\n");

	/* 
	 * check the result 
	 * X[i] have to be 1
	 */
	for (i = 0; i < MATRIX_SIZE; i++) {
		error = fabs(X[i] - 1.0f);

		if (max < error)
			max = error;
		if (error > 0.01f)
			printf("Result is on position %d wrong (%f != 1.0)\n",
			       i, X[i]);
	}
	printf("maximal error is %f\n", max);

	printf("\nmatrix size: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
	printf("number of iterations: %d\n", iterations);
	printf("Time : %lf sec\n",
	       (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec -
							      start.tv_usec) /
	       1000000.0);

	/* frees the allocated memory */
	free(X_old);
	free(X);
	clean_matrix(&A);
	clean_vector(&b);

	return 0;
}

