#include <stdio.h>
#include <stdlib.h>	 //for malloc
#include <pthread.h> //for threads

typedef struct ARG
{
	int Arow;
	int Bcol;
	int ans;
} args;

void *doThis(void *argss)
{
	args *temp = (args *)argss;
	temp->ans = 69;
	// printf("Hello from thread[%d] with bcol %d.\n", temp->Arow, temp->Bcol);
	return NULL;
}

int *A;
int **B;
int numRowA, numRowB;
int numColA, numColB;

int main()
{
	args *arguments; // dynamic number of arguments since the number of threads is unknown;
					 // pwede nyo tong gawing 2D array, mahihirapan lang kayo mag-loop
	pthread_t *tid;

	int total = 10;
	int testcases;
	int temp;

	FILE *fp;

	fp = fopen("matrix.in", "r");
	if (fp != NULL)
	{
		// read file here
		// you can use fscanf for reading the first and second lines
		// check if the size is invalid, i.e. colA != rowB
		fscanf(fp, "%d", &testcases);
		// read the matrix by repeated fscanf to an integer, then getchar until the matrix is filled or the end-of-file is reached
		fscanf(fp, "%d %d", &numRowA, &numColA);
		A = (int *) malloc(numRowA * numColA * sizeof(int));
		for (int i = 0; i < numRowA * numColA; i++)
		{
			fscanf(fp, "%d", &temp);
			printf("%d",temp);
			A[i] = temp;
		}
		// print Matrix A
		printf("matrix \n");
		for (int r = 0; r < numRowA; r++)
		{
			for (int c = 0; c < numColA; c++)
			{
				printf("%d ", A[r * numColA + c]);
			}
			printf("\n");
		}

		// print Matrix B

		// create your threads here. Pass to the thread the row of A and the column of B they need to check.
		tid = (pthread_t *)malloc(total * sizeof(pthread_t));
		arguments = (args *)malloc(total * sizeof(args));
		for (int i = 0; i < total; i++)
		{
			arguments[i].Arow = 1;
			arguments[i].Bcol = 2;
			pthread_create(&tid[i], NULL, doThis, (void *)&arguments[i]);
		}
		// join your threads here
		for (int i = 0; i < total; i++)
		{
			pthread_join(tid[i], NULL);
		}
		// manage the return values of the threads here

		// print the solution here
		for (int i = 0; i < total; i++)
		{
			printf("%d", arguments[i].ans);
		}
	}
	else
	{
		printf("File not found!\n");
	}
}