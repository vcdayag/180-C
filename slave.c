#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "interpolation.h"
#include "distributed.h"

#define MAX 11

int connectServer(char *ipadrress, int port)
{
	int sock;
	struct sockaddr_in server;

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket\n");
		return -1;
	}

	server.sin_addr.s_addr = inet_addr(ipadrress);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	// Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		return -1;
	}

	printf("Connected\n\n");
	return sock;
}

int main(int argc, char *argv[])
{
	int datainfo[3] = {MAX, 0, 0};
	int clientStatus;

	int sock;
	struct sockaddr_in server, master;

	clientinfo *configinfoarray = readConfig();

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket\n");
		return -1;
	}

	// Prepare the sockaddr_in structure
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));

	// Bind the socket
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		// print the error message
		printf("bind failed. Error");
		return 1;
	}
	printf("bind done");

	// listen to the socket
	listen(sock, 1);

	printf("Waiting for incoming connections...");
	int m = sizeof(struct sockaddr_in);

	// accept connection from an incoming client
	int master_sock = accept(sock, (struct sockaddr *)&master, (socklen_t *)&m);

	if (master_sock < 0)
	{
		printf("accept failed");
		return 1;
	}

	// STATUS: request corner matrix info
	// clientStatus = 0;
	// if (send(sock, &clientStatus, sizeof(int), 0) < 0)
	// {
	// 	printf("Send status failed\n");
	// 	return 1;
	// }

	// Receive a reply from the server
	int cornerMatrixInfo[3];
	if (recv(master_sock, &cornerMatrixInfo, 3 * sizeof(int), 0) < 0)
	{
		printf("recv failed\n");
		return 0;
	}
	printf("got info matrix");
	int nrow = cornerMatrixInfo[1] * 10 - 9;
	int ncol = cornerMatrixInfo[2] * 10 - 9;

	float *CORNERMATRIX = (float *)malloc(cornerMatrixInfo[0] * sizeof(float *));
	// Receive a cornermatrix from the server
	if (recv(master_sock, CORNERMATRIX, cornerMatrixInfo[0] * sizeof(float *), MSG_WAITALL) < 0)
	{
		printf("recv failed\n");
		return 0;
	}
	printf("got corner matrix");

	clientStatus = 1;
	if (send(master_sock, &clientStatus, sizeof(int), 0) < 0)
	{
		printf("Send status failed\n");
		return 1;
	}

	for (int i = 0; i < cornerMatrixInfo[0]; i++)
	{
		printf("%f ", CORNERMATRIX[i]);
	}

	printf("\n");

	// close the socket
	close(sock);
	printf("Socket closed\n");

	float **MATRIX;
	generateMatrixFromCorners(CORNERMATRIX, nrow);
	terrain_iter(nrow, ncol);
	printMatrix(nrow);

	// sock = connectServer(argv[1], atoi(argv[2]));
	// if (sock == -1)
	// {
	// 	printf("Connection error\n");
	// 	return -1;
	// }

	// clientStatus = 2;
	// if (send(sock, &clientStatus, sizeof(int), 0) < 0)
	// {
	// 	printf("Send status failed\n");
	// 	return 1;
	// }

	// printf("Client sent status.\n");
	// printf("Client finished.\n");

	return 0;
}
