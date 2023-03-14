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
            if (r % 10 && c % 10)
            {
                A[r][c] = rand() * 1001;
            }
            else
            {
                A[r][c] = 0;
            }
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
    args *arguments;
    pthread_t *tid;

    if (argc != 2)
    {
        printf("Must input an integer.");
        return 0;
    }

    n = atoi(argv[1]);

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