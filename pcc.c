#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

double* pearson_cor(int **matrix, int *vector, int n) {

    int sumOfX[n];
    int sumOfY;
    int xsquared[n][n];
    int ysquared[n];
    int sumOfxsquared[n];
    int sumOfysquared;
    int crossProduct[n][n];
    int sumOfCrossProduct[n];
    int sum = 0;
    // This is the vector for the answers (r)
    double r[n];

    //STEP 1
    //Compute for the sums
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            sum += matrix[i][j];
        }
        // printf("\n%d\n", sum);
        sumOfX[i] = sum;
        sum = 0;
    }

    for (int i=0; i<n ; i++) {
        sum += vector[i];
    }
    sumOfY = sum;
    sum = 0;

    //STEP 2
    //Calculate x^2
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            xsquared[i][j] = matrix[i][j]*matrix[i][j];
        }
    }

    //Calculate y^2
    for (int i=0; i<n ; i++) {
        ysquared[i] = vector[i]*vector[i];
    }

    //Compute for the sums of x^2(s) and y^2(s)
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            sum += xsquared[i][j];
        }
        // printf("\n%d\n", sum);
        sumOfxsquared[i] = sum;
        sum = 0;
    }

    for (int i=0; i<n ; i++) {
        sum += ysquared[i];
    }
    sumOfysquared = sum;
    sum = 0;

    //STEP 3
    //Calculate the cross product
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            crossProduct[i][j] = matrix[i][j]*vector[j];
        }
    }

    //Calculate the sum of the cross product
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            sum += crossProduct[i][j];
        }
        sumOfCrossProduct[i] = sum;
        sum = 0;
    }

    //STEP 4
    //Calculate r
    for (int i=0; i<n ; i++) {
            r[i] = (n*sumOfCrossProduct[i]-(sumOfX[i]*sumOfY)) / sqrt(( ((n*sumOfxsquared[i])-(sumOfX[i]*sumOfX[i])) * ((n*sumOfysquared)-(sumOfY*sumOfY)) )) ;
            printf("%f\n", r[i]);
    }

    return r;
}

int main()
{
    int n, count, i, j;
    printf("How big is the matrix? ");
    scanf("%d", &n);

    srand(time(NULL));

    int row = n;
    int col = n;

    // This is the matrix (X)
    int **matrix;
    matrix = malloc(n * sizeof *matrix);
    for (i=0; i<n; i++) {
        matrix[i] = malloc(n * sizeof *matrix[i]);
    }
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++)
            matrix[i][j] = (rand() % 100) + 1;
    }


    // This is the vector (y)
    int *vector;
    vector = (int*) malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        vector[i] = (rand() % 10) + 1;
    }

    time_t time_before = time(0);

    pearson_cor(matrix, vector, n);

    time_t time_after = time(0);
    printf("\nTIME ELAPSED: %ld\n", time_after-time_before);
        

    // FOR PRINTING OF THE VECTOR
    // for (i = 0; i < n; i++) {
    //     printf("%d ", vector[i]);
    // }

    // FOR PRINTING OF THE MATRIX
    // for (i = 0; i < row; i++) {
    //     for (j = 0; j < col; j++)
    //         printf("%d ", matrix[i][j]);
    //     printf("\n");
    // }

    return 0;
}