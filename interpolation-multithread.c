#include <stdio.h>
#include <stdlib.h> // for malloc
#define __USE_GNU
#include <pthread.h>  // for threads
#include <sys/time.h> // for gettimeofday
#include <unistd.h>   // for getting number of threads

#include "coreaffinity.h"

typedef struct ARG
{
    int rowStart;
    int rowEnd;
    int coreID;
} args;

float **MATRIX;
const int WEIGHTS[121][4] = {{0, 0, 0, 100}, {0, 0, 10, 90}, {0, 0, 20, 80}, {0, 0, 30, 70}, {0, 0, 40, 60}, {0, 0, 50, 50}, {0, 0, 60, 40}, {0, 0, 70, 30}, {0, 0, 80, 20}, {0, 0, 90, 10}, {0, 0, 100, 0}, {0, 10, 0, 90}, {1, 9, 9, 81}, {2, 8, 18, 72}, {3, 7, 27, 63}, {4, 6, 36, 54}, {5, 5, 45, 45}, {6, 4, 54, 36}, {7, 3, 63, 27}, {8, 2, 72, 18}, {9, 1, 81, 9}, {10, 0, 90, 0}, {0, 20, 0, 80}, {2, 18, 8, 72}, {4, 16, 16, 64}, {6, 14, 24, 56}, {8, 12, 32, 48}, {10, 10, 40, 40}, {12, 8, 48, 32}, {14, 6, 56, 24}, {16, 4, 64, 16}, {18, 2, 72, 8}, {20, 0, 80, 0}, {0, 30, 0, 70}, {3, 27, 7, 63}, {6, 24, 14, 56}, {9, 21, 21, 49}, {12, 18, 28, 42}, {15, 15, 35, 35}, {18, 12, 42, 28}, {21, 9, 49, 21}, {24, 6, 56, 14}, {27, 3, 63, 7}, {30, 0, 70, 0}, {0, 40, 0, 60}, {4, 36, 6, 54}, {8, 32, 12, 48}, {12, 28, 18, 42}, {16, 24, 24, 36}, {20, 20, 30, 30}, {24, 16, 36, 24}, {28, 12, 42, 18}, {32, 8, 48, 12}, {36, 4, 54, 6}, {40, 0, 60, 0}, {0, 50, 0, 50}, {5, 45, 5, 45}, {10, 40, 10, 40}, {15, 35, 15, 35}, {20, 30, 20, 30}, {25, 25, 25, 25}, {30, 20, 30, 20}, {35, 15, 35, 15}, {40, 10, 40, 10}, {45, 5, 45, 5}, {50, 0, 50, 0}, {0, 60, 0, 40}, {6, 54, 4, 36}, {12, 48, 8, 32}, {18, 42, 12, 28}, {24, 36, 16, 24}, {30, 30, 20, 20}, {36, 24, 24, 16}, {42, 18, 28, 12}, {48, 12, 32, 8}, {54, 6, 36, 4}, {60, 0, 40, 0}, {0, 70, 0, 30}, {7, 63, 3, 27}, {14, 56, 6, 24}, {21, 49, 9, 21}, {28, 42, 12, 18}, {35, 35, 15, 15}, {42, 28, 18, 12}, {49, 21, 21, 9}, {56, 14, 24, 6}, {63, 7, 27, 3}, {70, 0, 30, 0}, {0, 80, 0, 20}, {8, 72, 2, 18}, {16, 64, 4, 16}, {24, 56, 6, 14}, {32, 48, 8, 12}, {40, 40, 10, 10}, {48, 32, 12, 8}, {56, 24, 14, 6}, {64, 16, 16, 4}, {72, 8, 18, 2}, {80, 0, 20, 0}, {0, 90, 0, 10}, {9, 81, 1, 9}, {18, 72, 2, 8}, {27, 63, 3, 7}, {36, 54, 4, 6}, {45, 45, 5, 5}, {54, 36, 6, 4}, {63, 27, 7, 3}, {72, 18, 8, 2}, {81, 9, 9, 1}, {90, 0, 10, 0}, {0, 100, 0, 0}, {10, 90, 0, 0}, {20, 80, 0, 0}, {30, 70, 0, 0}, {40, 60, 0, 0}, {50, 50, 0, 0}, {60, 40, 0, 0}, {70, 30, 0, 0}, {80, 20, 0, 0}, {90, 10, 0, 0}, {100, 0, 0, 0}};

void printMatrix(int n)
{
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            printf("%0.1lf ", MATRIX[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

void generateMatrix(int n)
{
    srand((unsigned)time(NULL));

    MATRIX = (float **)malloc(n * sizeof(float *));
    for (int r = 0; r < n; r++)
    {
        MATRIX[r] = (float *)malloc(n * sizeof(float));
        for (int c = 0; c < n; c++)
        {
            if (r % 10 == 0 && c % 10 == 0)
            {
                MATRIX[r][c] = rand() % 1000 + 1;
                // MATRIX[r][c] = 420 + r / 10 + c / 10;
            }
            else
            {
                MATRIX[r][c] = 0;
            }
        }
    }
    return;
}

int n, t;
void *terrain_iter(void *argss)
{
    args *arguments = (args *)argss;

#ifdef MANUALAFFINITY
    const pthread_t pid = pthread_self();
    const int core_id = arguments->coreID;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    const int set_result = pthread_setaffinity_np(pid, sizeof(cpu_set_t), &cpuset);
#endif

    int LRPOINTrow, LRPOINTcol;
    for (int row = arguments->rowStart; row < arguments->rowEnd; row++)
    {
        LRPOINTrow = row - (row % 10);
        if (row % 10 == 0 && row != 0)
            LRPOINTrow -= 10;
        for (int col = 0; col < n; col++)
        // for (int col = arguments->colStart; col < arguments->colEnd; col++)
        {
            if (col % 10 == 0)
            {
                LRPOINTcol = col - (col % 10);
                if (col != 0)
                    LRPOINTcol -= 10;
                if (row % 10 == 0)
                    continue;
            }
            int index = row % 10 + col % 10;
            int d = WEIGHTS[index][0];
            int c = WEIGHTS[index][1];
            int b = WEIGHTS[index][2];
            int a = WEIGHTS[index][3];

            int A = MATRIX[LRPOINTrow][LRPOINTcol];
            int B = MATRIX[LRPOINTrow][LRPOINTcol + 10];
            int C = MATRIX[LRPOINTrow + 10][LRPOINTcol];
            int D = MATRIX[LRPOINTrow + 10][LRPOINTcol + 10];

            MATRIX[row][col] = (a * A + b * B + c * C + d * D) / 100.0;
        }
    }
}

int main(int argc, char *argv[])
{

    struct timeval time_before, time_after;

    if (argc != 3)
    {
        printf("Must input an integer.");
        return 0;
    }

    n = atoi(argv[1]) + 1;
    t = atoi(argv[2]);

    int numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);

    generateMatrix(n);

    pthread_t *tid = (pthread_t *)malloc(t * sizeof(pthread_t));
    args *arguments = (args *)malloc(t * sizeof(args));

    int numberOfRows = n / t;
    int computedTotal = numberOfRows * t;
    int toDistribute = n - computedTotal;

    int previousRowStart = 0;
    int core_id = 0;
    for (int thread = 0; thread < t; thread++)
    {
        arguments[thread].rowStart = previousRowStart;
        arguments[thread].rowEnd = previousRowStart + numberOfRows;

#ifdef MANUALAFFINITY
        arguments[thread].coreID = thread % numberOfProcessors;
#endif

        if (toDistribute != 0)
        {
            toDistribute--;
            arguments[thread].rowEnd += 1;
        }
        else if (thread + 1 == t)
        {
            arguments[thread].rowEnd = n;
        }

        previousRowStart = arguments[thread].rowEnd;
    }

    gettimeofday(&time_before, 0);

    for (int thread = 0; thread < t; thread++)
    {
        pthread_create(&tid[thread], NULL, terrain_iter, (void *)&arguments[thread]);
    }
    // join your threads here
    for (int thread = 0; thread < t; thread++)
    {
        pthread_join(tid[thread], NULL);
    }

    gettimeofday(&time_after, 0);

    long seconds = time_after.tv_sec - time_before.tv_sec;
    long microseconds = time_after.tv_usec - time_before.tv_usec;
    double time_elapsed = seconds + microseconds * 1e-6;

    printf("Time elapsed: %f seconds.\n", time_elapsed);

    // printMatrix(n);

    // free MATRIX
    for (int i = 0; i < n; i++)
        free(MATRIX[i]);
    free(MATRIX);
    // free tid and arguments
    free(tid);
    free(arguments);
}