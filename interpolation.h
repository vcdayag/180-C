#ifndef INTERPOLATIONH /* This is an "include guard" */
#define INTERPOLATIONH /* prevents the file from being included twice. */
                       /* Including a header file twice causes all kinds */
                       /* of interesting problems.*/

#include <stdio.h>
#include <pthread.h>  // for threads
#include <stdlib.h>   // for malloc
#include <sys/time.h> // for gettimeofday

void printMatrix(int n);
void generateMatrix(int n);
void terrain_iter(int n);
float *generateCornerMatrix(int n);

#endif /* INTERPOLATIONH */