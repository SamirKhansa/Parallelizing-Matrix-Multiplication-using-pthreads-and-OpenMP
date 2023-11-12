#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define MATRIX_DIM 1024
#define BLOCK_SIZE 128
#define NUM_THREADS (MATRIX_DIM / BLOCK_SIZE)

double X[MATRIX_DIM][MATRIX_DIM], Y[MATRIX_DIM][MATRIX_DIM], Z[MATRIX_DIM][MATRIX_DIM];
pthread_t thread_pool[NUM_THREADS][NUM_THREADS];
int thread_args[NUM_THREADS][NUM_THREADS];

typedef struct {
    int row;
    int col;
} BlockInfo;

void *matrix_multiply_block(void *arg) {
    BlockInfo *info = (BlockInfo *)arg;
    int row = info->row;
    int col = info->col;

    int i, j, k;
    double result;

    for (i = row * BLOCK_SIZE; i < (row + 1) * BLOCK_SIZE; i++) {
        for (j = col * BLOCK_SIZE; j < (col + 1) * BLOCK_SIZE; j++) {
            result = 0.0;
            for (k = 0; k < MATRIX_DIM; k++) {
                result += X[i][k] * Y[k][j];
            }
            Z[i][j] = result;
        }
    }

    pthread_exit(NULL);
}

int main() {
    int i, j;
    void *status;

    for (i = 0; i < MATRIX_DIM; i++) {
        for (j = 0; j < MATRIX_DIM; j++) {
            X[i][j] = i + j;
            Y[i][j] = i - j;
            Z[i][j] = 0;
        }
    }

    double start_time = omp_get_wtime();

    for (i = 0; i < NUM_THREADS; i++) {
        for (j = 0; j < NUM_THREADS; j++) {
            thread_args[i][j] = i;
            BlockInfo *info = malloc(sizeof(BlockInfo));
            info->row = i;
            info->col = j;
            pthread_create(&thread_pool[i][j], NULL, matrix_multiply_block, (void *)info);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        for (j = 0; j < NUM_THREADS; j++) {
            pthread_join(thread_pool[i][j], &status);
        }
    }

    double end_time = omp_get_wtime();
    printf("%.6f seconds\n", end_time - start_time);

    return 0;
}
