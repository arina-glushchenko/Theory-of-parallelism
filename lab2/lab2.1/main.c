#include <stdio.h>
#include <omp.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#define nthreads 40
#define m 20000
#define n 20000

/*
* matrix_vector_product: Compute matrix-vector product c[m] = a[m][n] * b[n]
*/
void matrix_vector_product(double* a, double* b, double* c)
{
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++)
			c[i] += a[i * n + j] * b[j];

	}
}

double run_serial()
{
	double* a, * b, * c;
	a = malloc(sizeof(*a) * m * n);
	b = malloc(sizeof(*b) * n);
	c = malloc(sizeof(*c) * m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;

	}
	for (int j = 0; j < n; j++)
		b[j] = j;
	double t = omp_get_wtime();
	matrix_vector_product(a, b, c);
	t = omp_get_wtime() - t;
	printf("Elapsed time (serial): %.6f sec.\n", t);
	free(a);
	free(b);
	free(c);
	return t;
}

/* matrix_vector_product_omp: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void matrix_vector_product_omp(double* a, double* b, double* c)
{
#pragma omp parallel num_threads(nthreads)
	{
		//int nthreads = omp_get_num_threads();
		int threadid = omp_get_thread_num();
		int items_per_thread = m / nthreads;
		int lb = threadid * items_per_thread;
		int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
		for (int i = lb; i <= ub; i++) {
			c[i] = 0.0;
			for (int j = 0; j < n; j++)
				c[i] += a[i * n + j] * b[j];

		}
	}
}


double run_parallel()
{
	double* a, * b, * c;
	// Allocate memory for 2-d array a[m, n]
	a = malloc(sizeof(*a) * m * n);
	b = malloc(sizeof(*b) * n);
	c = malloc(sizeof(*c) * m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;

	}
	for (int j = 0; j < n; j++)
		b[j] = j;
	double t = omp_get_wtime();
	matrix_vector_product_omp(a, b, c);
	t = omp_get_wtime() - t;
	printf("Elapsed time (parallel): %.6f sec.\n", t);
	free(a);
	free(b);
	free(c);
	return t;
}

int main(int argc, char** argv)
{

	printf("Matrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
	printf("Memory used: %" PRIu64 " MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);
	double ts = run_serial();
	double tp = run_parallel();
	printf("SpeedUp: %.2f\n", ts / tp);

	return 0;
}