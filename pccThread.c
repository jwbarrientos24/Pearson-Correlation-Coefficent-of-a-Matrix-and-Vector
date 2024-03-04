#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Structure to pass parameters to the thread function
struct ThreadArgs {
    float **matrix;
    float *vector;
    int n;
    int thread_id;
    int num_threads;
    double *r;
};

// Function to compute Pearson correlation coefficients in parallel
void *compute_correlation(void *args) {
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    float **matrix = thread_args->matrix;
    float *vector = thread_args->vector;
    int n = thread_args->n;
    int thread_id = thread_args->thread_id;
    int num_threads = thread_args->num_threads;
    double *r = thread_args->r;

    // Compute the range of rows for this thread
    int chunk_size = n / num_threads;
    int start_row = thread_id * chunk_size;
    int end_row = (thread_id == num_threads - 1) ? n : (thread_id + 1) * chunk_size;

    // Compute Pearson correlation coefficients for the assigned rows
    for (int i = start_row; i < end_row; i++) {
        float sum = 0, sumOfX = 0, sumOfY = 0, sumOfxsquared = 0, sumOfysquared = 0, sumOfCrossProduct = 0;
        
        // Compute sums and products
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * vector[j];
            sumOfX += matrix[i][j];
            sumOfY += vector[j];
            sumOfxsquared += matrix[i][j] * matrix[i][j];
            sumOfysquared += vector[j] * vector[j];
        }
        
        // Calculate the cross product
        sumOfCrossProduct = sum;
        
        // Calculate r
        r[i] = (n * sumOfCrossProduct - (sumOfX * sumOfY)) / sqrt(((n * sumOfxsquared - (sumOfX * sumOfX)) * (n * sumOfysquared - (sumOfY * sumOfY))));
    }

    pthread_exit(NULL);
}

// Function to compute Pearson correlation coefficients using threads
double *pearson_cor_parallel(float **matrix, float *vector, int n, int num_threads) {
    // Allocate memory for the correlation coefficients
    double *r = (double *)malloc(n * sizeof(double));

    // Array to hold thread IDs
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

    // Array to hold thread arguments
    struct ThreadArgs *thread_args = (struct ThreadArgs *)malloc(num_threads * sizeof(struct ThreadArgs));

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].matrix = matrix;
        thread_args[i].vector = vector;
        thread_args[i].n = n;
        thread_args[i].thread_id = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].r = r;
        pthread_create(&threads[i], NULL, compute_correlation, (void *)&thread_args[i]);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free memory
    free(threads);
    free(thread_args);

    return r;
}

int main() {
    int n;

    printf("How big is the matrix? ");
    scanf("%d", &n);

    srand(time(NULL));

    // Allocate memory for the matrix
    float **matrix;
    matrix = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
    }

    // Populate the matrix with random values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (rand() % 100) + 1;
        }
    }

    // Allocate memory for the vector
    float *vector;
    vector = (float *)malloc(n * sizeof(float));

    // Populate the vector with random values
    for (int i = 0; i < n; i++) {
        vector[i] = (rand() % 10) + 1;
    }

    int num_threads;
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    time_t time_before = time(0);

    // Compute Pearson correlation coefficients in parallel
    double *r = pearson_cor_parallel(matrix, vector, n, num_threads);

    time_t time_after = time(0);
    printf("\nTIME ELAPSED: %ld\n", time_after-time_before);

    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(r);

    return 0;
}
