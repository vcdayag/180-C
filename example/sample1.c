/**
 sample1.c
 KBPPelaez

 Sample program demonstrating the usage of the basic pthread functions:
 	* pthread_create()
	* pthread_join()
	* pthread_exit()

 Program Flow	
	* Five (5) threads will be created
	* Specific function will be called by each thread
		- Ex 1: noParameter()
		- Ex 2: oneParameter()
		- Ex 3: multiParameter()
**/

#include<stdio.h>
#include<pthread.h>

//structure that will be passed as parameter to the thread
typedef struct node{
	int num;
	char letter;
}arg;

//function for the thread that takes two or more parameters
void * multiParameter(void *args){
	arg * temp;

	temp = (arg *) args;

	printf("Hello from thread[%d] with letter %c.\n",temp->num, temp->letter);

	pthread_exit(NULL);

}

//function for the thread that takes one parameter
void * oneParameter(void *args){
	int *num;
	num = (int *) args;

	printf("Hello from thread[%d]\n", *num);

	pthread_exit(NULL);
}

//function for the thread that takes processes 0 arguments
void * noParameter(void *args){
	printf("Hello\n");

	pthread_exit(NULL);
}

int main(){
	pthread_t tid[5];	 //pthread data type for the thread ids
	int i, threadnum[5];
	arg arguments[5];

	for(i=0; i<5; i+=1){

		// Ex 1: Each thread calls a function with NO (0) parameter
		// pthread_create(&tid[i], NULL, noParameter, NULL);

		// Ex 2: Each thread calls a function with ONE (1) parameter
		// threadnum[i] = i;
		// pthread_create(&tid[i], NULL, oneParameter, (void *) &threadnum[i]);

		// Ex 3: Each thread calls a function with TWO (2) parameters
		// arguments[i].num = i;
		// arguments[i].letter = i+65;
		// pthread_create(&tid[i], NULL, multiParameter, (void *) &arguments[i]);
	}

	for(i=0; i<5; i+=1){
		//joins the threads so that the kernel thread will wait for the others
		pthread_join(tid[i], NULL);
	}

	return 0;

}//end of main