#define _GNU_SOURCE

/* 
References:
J. P. Barrientos, “jwbarrientos24/Pearson-Correlation-Coefficent-of-a-Matrix-and-Vector,” GitHub, Feb. 25, 2024. https://github.com/jwbarrientos24/Pearson-Correlation-Coefficent-of-a-Matrix-and-Vector (accessed Mar. 04, 2024).
“How to set CPU affinity of a particular pthread?,” Stack Overflow. https://stackoverflow.com/questions/1407786/how-to-set-cpu-affinity-of-a-particular-pthread (accessed Mar. 19, 2024).
Amiri, “How to set CPU affinity for a process from C or C++ in Linux?,” Stack Overflow, Sep. 02, 2020. https://stackoverflow.com/a/41299791 (accessed Mar. 19, 2024).
M. Kerrisk, “pthread_attr_setaffinity_np(3) - Linux manual page,” man7.org, Dec. 22, 2023. https://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html (accessed Mar. 19, 2024).
“CPU Affinity (The GNU C Library),” www.gnu.org. https://www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html
M. Kerrisk, “CPU_SET(3) - Linux manual page,” man7.org, Dec. 22, 2023. https://man7.org/linux/man-pages/man3/CPU_SET.3.html (accessed Mar. 19, 2024).
M. Kerrisk, “pthread_create(3) - Linux manual page,” man7.org, Dec. 22, 2023. https://man7.org/linux/man-pages/man3/pthread_create.3.html
*/


#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sched.h> // For CPU affinity
#include <unistd.h> // For sysconf

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
    int end_row;
    if (thread_id == num_threads - 1) {
        end_row = n;
    } else {
        end_row = (thread_id + 1) * chunk_size;
    }

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

    // Get the number of CPU cores
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);

    // Create threads
    // Assign threads to cores
    int thread_number = 0;
    for (int i = 0; i < num_threads; i++) {
        // Initialize thread arguments for the current thread
        thread_args[i].matrix = matrix;
        thread_args[i].vector = vector;
        thread_args[i].n = n;
        thread_args[i].thread_id = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].r = r;

        // Set CPU affinity for the current thread
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(thread_number % (num_cores - 1), &cpuset); // Assign cores
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        pthread_create(&threads[i], &attr, compute_correlation, (void *)&thread_args[i]);
        thread_number++;
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
    struct timeval start_time, end_time;

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

    gettimeofday(&start_time, NULL);

    // Compute Pearson correlation coefficients in parallel
    double *r = pearson_cor_parallel(matrix, vector, n, num_threads);

    gettimeofday(&end_time, NULL);
    double time_elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000.0; // Seconds to milliseconds
    time_elapsed += (end_time.tv_usec - start_time.tv_usec) / 1000.0; // Microseconds to milliseconds
    time_elapsed = time_elapsed / 1000; // Milliseconds converted to seconds.milliseconds

    printf("\nTIME ELAPSED: %.2f seconds\n", time_elapsed);

    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(r);

    return 0;
}
