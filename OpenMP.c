#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MATRIX_DIM 1024
#define NUM_THREADS 8

int matrix_a[MATRIX_DIM][MATRIX_DIM];
int matrix_b[MATRIX_DIM][MATRIX_DIM];
long int result_matrix[MATRIX_DIM][MATRIX_DIM];

int main() {
    printf("Hello world");
    int i, j, k;
    

    // Initialize matrices
    for (i = 0; i < MATRIX_DIM; ++i) {
        for (j = 0; j < MATRIX_DIM; ++j) {
            matrix_a[i][j] = i + j;
            matrix_b[i][j] = i - j;
            result_matrix[i][j] = 0;
        }
    }

    double startOftime = omp_get_wtime();

    #pragma omp parallel for private(i, j, k) shared(matrix_a, matrix_b, result_matrix) schedule(dynamic)
    for (i = 0; i < MATRIX_DIM; ++i) {
        for (j = 0; j < MATRIX_DIM; ++j) {
            for (k = 0; k < MATRIX_DIM; ++k) {  // Fix: use MATRIX_DIM instead of NUM_THREADS
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }

    double endOftime = omp_get_wtime();
    printf("OpenMP total time: %f seconds\n", endOftime - startOftime);

    return 0;
}
