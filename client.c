// Client code in C to sort the array
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 11
// Driver code
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in server;
    int datainfo[3] = {MAX,0,0};
    
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(5050);

	// Connect to remote server
	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	// Receive a reply from the server
	int arraysize[1];
	if (recv(sock, &arraysize, 1 * sizeof(int), 0) < 0) {
		puts("recv failed");
		return 0;
	}

    puts("Server reply :\n");
	printf("%d\n", arraysize[0]);

	for (int i = 0; i < 3; i++)
	{
		datainfo[i] = 69;
	}

    if (send(sock, &datainfo, 3 * sizeof(int), 0) < 0) {
		puts("Send failed");
		return 1;
	}

	float* CORNERMATRIX = ( float* ) malloc(arraysize[0] * arraysize[0] * sizeof(float *));

	// Receive a reply from the server
	if (recv(sock, CORNERMATRIX, arraysize[0] * arraysize[0] * sizeof(float), 0) < 0) {
		puts("recv failed");
		return 0;
	}

	puts("Server reply :\n");
	for (int i = 0; i <  arraysize[0] * arraysize[0]; i++) {
		printf("%f\n", CORNERMATRIX[i]);
	}

	// close the socket
	close(sock);
	return 0;
}
