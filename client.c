#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "interpolation.h"
#include "distributed.h"

#define MAX 11

int connectServer(char* ipadrress, int port)
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
	int sock;
	int datainfo[3] = {MAX, 0, 0};
	int clientStatus;

	sock = connectServer(argv[1],atoi(argv[2]));
	if (sock == -1)
	{
		printf("Connection error\n");
		return -1;
	}

	// STATUS: request corner matrix info
	clientStatus = 0;
	if (send(sock, &clientStatus, sizeof(int), 0) < 0)
	{
		printf("Send status failed\n");
		return 1;
	}

	// Receive a reply from the server
	int cornerMatrixInfo[3];
	if (recv(sock, &cornerMatrixInfo, 3 * sizeof(int), 0) < 0)
	{
		printf("recv failed\n");
		return 0;
	}
	int nrow = cornerMatrixInfo[1] * 10 - 9;
	int ncol = cornerMatrixInfo[2] * 10 - 9;

	clientStatus = 1;
	if (send(sock, &clientStatus, sizeof(int), 0) < 0)
	{
		printf("Send status failed\n");
		return 1;
	}

	float *CORNERMATRIX = (float *)malloc(cornerMatrixInfo[0] * sizeof(float *));
	// Receive a cornermatrix from the server
	if (recv(sock, CORNERMATRIX, cornerMatrixInfo[0] * sizeof(float *), MSG_WAITALL) < 0)
	{
		printf("recv failed\n");
		return 0;
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

	sock = connectServer(argv[1],atoi(argv[2]));
	if (sock == -1)
	{
		printf("Connection error\n");
		return -1;
	}

	clientStatus = 2;
	if (send(sock, &clientStatus, sizeof(int), 0) < 0)
	{
		printf("Send status failed\n");
		return 1;
	}

	printf("Client sent status.\n");
	printf("Client finished.\n");

	return 0;
}
