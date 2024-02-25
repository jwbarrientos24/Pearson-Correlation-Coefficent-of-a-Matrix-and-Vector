/*
THIS IS A SAMPLE OF THE CODE IN WHICH WE TEST OUT THE COMPUTATION ON A FIXED SET OF DATA.
THE SAMPLE DATASET IS TAKEN FROM THIS PAGE:
https://www.scribbr.com/statistics/pearson-correlation-coefficient/
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

double* pearson_cor(float matrix[10][10], float vector[10], int n) {

    float sumOfX[n];
    float sumOfY;
    float xsquared[n][n];
    float ysquared[n];
    float sumOfxsquared[n];
    float sumOfysquared;
    float crossProduct[n][n];
    float sumOfCrossProduct[n];
    float sum = 0;
    // This is the vector for the answers (r)
    double r[n];

    //STEP 1
    //Compute for the sums
    for (int i=0; i<n ; i++) {
        for (int j=0; j<n; j++) {
            sum += matrix[i][j];
        }
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
    printf("Here are the results: \n");
    for (int i=0; i<n ; i++) {
            r[i] = (n*sumOfCrossProduct[i]-(sumOfX[i]*sumOfY)) / sqrt(( ((n*sumOfxsquared[i])-(sumOfX[i]*sumOfX[i])) * ((n*sumOfysquared)-(sumOfY*sumOfY)) )) ;
            printf("%f\n", r[i]);
    }

    return r;
}

float main()
{
    int n = 10, count, i, j;
    printf("The matrix is 10x10 with fixed data\n");
    float matrix[10][10];
    float vector[10];

    srand(time(NULL));


    //THIS IS THE MATRIX FOR THE BABIES
    for (i = 0; i < 10; i++) {
        matrix[i][0] = 3.63;
        matrix[i][1] = 3.02;
        matrix[i][2] = 3.82;
        matrix[i][3] = 3.42;
        matrix[i][4] = 3.59;
        matrix[i][5] = 2.87;
        matrix[i][6] = 3.03;
        matrix[i][7] = 3.46;
        matrix[i][8] = 3.36;
        matrix[i][9] = 3.3;
    }

    //THIS IS THE vector FOR THE BABIES
    vector[0] = 53.1;
    vector[1] = 49.7;
    vector[2] = 48.4;
    vector[3] = 54.2;
    vector[4] = 54.9;
    vector[5] = 43.7;
    vector[6] = 47.2;
    vector[7] = 45.2;
    vector[8] = 54.4;
    vector[9] = 50.4;

    float row = n;
    float col = n;


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