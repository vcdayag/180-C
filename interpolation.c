#include <pthread.h> //for threads
#include <stdio.h>
#include <stdlib.h> //for malloc

typedef struct ARG
{
    int n;
    int rowStart;
    int rowEnd;
    int colStart;
    int colEnd;
} args;

int n;
int **A;
int numRowA;
int numColA;

void generateMatrix(int n)
{
    A = (int **)malloc(numRowA * sizeof(int *));
    for (int r = 0; r < numRowA; r++)
    {
        A[r] = (int *)malloc(numColA * sizeof(int));
        for (int c = 0; c < numColA; c++)
        {
            A[r][c] = 1;
        }
    }
    return;
}

void *terrain_iter(void *argss)
{
    args *arguments = (args *)argss;
    for (int i = 0; i < arguments->n; i++)
    {
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("Must input an integer.");
        return 0;
    }

    args *arguments; // dynamic number of arguments since the number of threads is unknown;
                     // pwede nyo tong gawing 2D array, mahihirapan lang kayo mag-loop
    pthread_t *tid;
    int testcases;
    FILE *fp;

    for (int testcase = 0; testcase < testcases; testcase++)
    {

        // create your threads here. Pass to the thread the row of A and the column of B they need to check.
        tid = (pthread_t *)malloc(n * n * sizeof(pthread_t));
        arguments = (args *)malloc(n * n * sizeof(args));
        for (int r = 0; r < n; r++)
        {
            for (int c = 0; c < n; c++)
            {
                int i = r * n + c;
                pthread_create(&tid[i], NULL, terrain_iter, (void *)&arguments[i]);
            }
        }
        // join your threads here
        for (int i = 0; i < n * n; i++)
        {
            pthread_join(tid[i], NULL);
        }

        // manage the return values of the threads here
        // print the solution here
        // printf("\nResulting Matrix:\n");
        // for (int r = 0; r < numRowB; r++) {
        //     for (int c = 0; c < numColB; c++) {
        //         printf("%d ", arguments[r * numColB + c].ans);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        free(A);
        free(arguments);
        free(tid);
    }
}