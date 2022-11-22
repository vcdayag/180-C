#include <pthread.h>  //for threads
#include <stdio.h>
#include <stdlib.h>  //for malloc

typedef struct ARG {
    int Arow;
    int Bcol;
    int ans;
} args;

int **A;
int **B;
int numRowA, numRowB;
int numColA, numColB;

void *computeCellMultiplication(void *argss) {
    args *argument = (args *)argss;
    for (int i = 0; i < numRowB; i++) {
        argument->ans += A[argument->Arow][i] * B[i][argument->Bcol];
    }
    return NULL;
}

int main() {
    args *arguments;  // dynamic number of arguments since the number of threads is unknown;
                      // pwede nyo tong gawing 2D array, mahihirapan lang kayo mag-loop
    pthread_t *tid;

    int total = 10;
    int testcases;
    int temp;

    FILE *fp;

    fp = fopen("matrix.in", "r");
    if (fp != NULL) {
        // read file here
        // you can use fscanf for reading the first and second lines
        // check if the size is invalid, i.e. colA != rowB
        fscanf(fp, "%d", &testcases);
        for (int testcase = 0; testcase < testcases; testcase++) {
            printf("Testcase #%d\n", testcase + 1);
            // read the matrix by repeated fscanf to an integer, then getchar until the matrix is filled or the end-of-file is reached
            fscanf(fp, "%d %d", &numRowA, &numColA);
            A = (int **)malloc(numRowA * sizeof(int *));
            for (int r = 0; r < numRowA; r++) {
                A[r] = (int *)malloc(numColA * sizeof(int));
                for (int c = 0; c < numColA; c++) {
                    fscanf(fp, "%d", &A[r][c]);
                }
            }
            // print Matrix A
            printf("matrix A\n");
            for (int r = 0; r < numRowA; r++) {
                for (int c = 0; c < numColA; c++) {
                    printf("%d ", A[r][c]);
                }
                printf("\n");
            }

            // print Matrix B
            fscanf(fp, "%d %d", &numRowB, &numColB);
            B = (int **)malloc(numRowB * sizeof(int *));
            for (int r = 0; r < numRowB; r++) {
                B[r] = (int *)malloc(numColB * sizeof(int));
                for (int c = 0; c < numColB; c++) {
                    fscanf(fp, "%d", &B[r][c]);
                }
            }

            printf("\nmatrix B\n");
            for (int r = 0; r < numRowB; r++) {
                for (int c = 0; c < numColB; c++) {
                    printf("%d ", B[r][c]);
                }
                printf("\n");
            }

            if (numColA != numRowB) {
                printf("Column of A is not equal to Row of B");
                printf("Can't do matrix multiplication");
                return 0;
            }

            // create your threads here. Pass to the thread the row of A and the column of B they need to check.
            tid = (pthread_t *)malloc(numRowB * numColB * sizeof(pthread_t));
            arguments = (args *)malloc(numRowB * numColB * sizeof(args));
            for (int r = 0; r < numRowB; r++) {
                for (int c = 0; c < numColB; c++) {
                    int i = r * numColB + c;
                    arguments[i].Arow = r;
                    arguments[i].Bcol = c;
                    pthread_create(&tid[i], NULL, computeCellMultiplication, (void *)&arguments[i]);
                }
            }
            // join your threads here
            for (int i = 0; i < total; i++) {
                pthread_join(tid[i], NULL);
            }
            
            // manage the return values of the threads here
            // print the solution here
            printf("\nResulting Matrix:\n");
            for (int r = 0; r < numRowB; r++) {
                for (int c = 0; c < numColB; c++) {
                    printf("%d ", arguments[r * numColB + c].ans);
                }
                printf("\n");
            }
            printf("\n");
        }
        fclose(fp);
    } else {
        printf("File not found!\n");
    }
}