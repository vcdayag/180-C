#ifndef INTERPOLATIONH /* This is an "include guard" */
#define INTERPOLATIONH /* prevents the file from being included twice. */
                       /* Including a header file twice causes all kinds */
                       /* of interesting problems.*/

#include <stdio.h>
#include <pthread.h>  // for threads
#include <stdlib.h>   // for malloc
#include <sys/time.h> // for gettimeofday

extern float **MATRIX;

void printMatrix(int n);
void generateMatrix(int n);
void terrain_iter(int nrow, int ncol);
float *generateCornerMatrix(int n);
void generateMatrixFromCorners(float* CORNERMATRIX, int n);

#endif /* INTERPOLATIONH */