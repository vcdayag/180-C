#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "interpolation.h"
#include "distributed.h"

int main(int argc, char *argv[])
{
	int sock, clientStatus;
	struct sockaddr_in server, master;

	printf("%d", atoi(argv[1]));

	// clientinfo *configinfoarray = readConfig();

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

	while (1)
	{
		printf("Waiting for incoming connections...");
		int m = sizeof(struct sockaddr_in);

		// accept connection from an incoming client
		int master_sock = accept(sock, (struct sockaddr *)&master, (socklen_t *)&m);

		if (master_sock < 0)
		{
			printf("accept failed");
			return 1;
		}

		// Receive a reply from the server
		int cornerMatrixInfo[3];
		if (recv(master_sock, &cornerMatrixInfo, 3 * sizeof(int), 0) < 0)
		{
			printf("recv failed\n");
			return 0;
		}
		printf("got info matrix\n");
		int nrow = cornerMatrixInfo[1] * 10 - 9;
		int ncol = cornerMatrixInfo[2] * 10 - 9;

		float *CORNERMATRIX = (float *)malloc(cornerMatrixInfo[0] * sizeof(float *));

		// Receive a cornermatrix from the server
		if (recv(master_sock, CORNERMATRIX, cornerMatrixInfo[0] * sizeof(float *), MSG_WAITALL) < 0)
		{
			printf("recv failed\n");
			return 0;
		}
		printf("got corner matrix\n");

		clientStatus = 1;
		if (send(master_sock, &clientStatus, sizeof(int), 0) < 0)
		{
			printf("Send status failed\n");
			return 1;
		}

		// for (int i = 0; i < cornerMatrixInfo[0]; i++)
		// {
		// 	printf("%f ", CORNERMATRIX[i]);
		// }

		// printf("\n");

		// close the socket
		printf("Socket closed\n");

		float **MATRIX;
		generateMatrixFromCorners(CORNERMATRIX, nrow);
		terrain_iter(nrow, ncol);
		printMatrix(nrow);
		printf("finished interpolation.\n");
	}
	close(sock);

	return 0;
}
