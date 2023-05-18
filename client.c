// Client code in C to sort the array
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "interpolation.h"

#define MAX 11
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	int datainfo[3] = {MAX, 0, 0};

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(5600);

	// Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	// Receive a reply from the server
	int cornerMatrixInfo[3];
	if (recv(sock, &cornerMatrixInfo, 3 * sizeof(int), 0) < 0)
	{
		puts("recv failed");
		return 0;
	}

	int nrow = cornerMatrixInfo[1] * 10 - 9;
	int ncol = cornerMatrixInfo[2] * 10 - 9;

	printf("%d", cornerMatrixInfo[0]);

	puts("Server reply :\n");
	if (send(sock, &datainfo, 3 * sizeof(int), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}

	float *CORNERMATRIX = (float *)malloc(cornerMatrixInfo[0] * sizeof(float *));
	// Receive a cornermatrix from the server
	if (recv(sock, CORNERMATRIX, cornerMatrixInfo[0] * sizeof(float *), MSG_WAITALL) < 0)
	{
		puts("recv failed");
		return 0;
	}

	puts("Server reply :\n");
	for (int i = 0; i < cornerMatrixInfo[0]; i++)
	{
		printf("%f ", CORNERMATRIX[i]);
	}

	printf("\n");

	float **MATRIX;
	generateMatrixFromCorners(CORNERMATRIX, nrow);
	terrain_iter(nrow, ncol);
	printMatrix(nrow);

	// close the socket
	close(sock);
	return 0;
}
