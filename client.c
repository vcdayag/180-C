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
    int* integers;
    int datainfo[3] = {MAX,0,0};
    
    integers = ( int* ) malloc(MAX * sizeof(int));
    for (int i = 0; i < MAX; i++)
    {
        integers[i]= i;
    }
    integers[8]= 69;
    

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

	if (send(sock, &datainfo, 3 * sizeof(int), 0) < 0) {
		puts("Send failed");
		return 1;
	}

	// Receive a reply from the server
	if (recv(sock, &datainfo, 3 * sizeof(int), 0) < 0) {
		puts("recv failed");
		return 0;
	}

    puts("Server reply :\n");
	for (int i = 0; i < 3; i++) {
		printf("%d\n", datainfo[i]);
	}

    if (send(sock, integers, MAX * sizeof(int), 0) < 0) {
		puts("Send failed");
		return 1;
	}

	// Receive a reply from the server
	if (recv(sock, integers, MAX * sizeof(int), 0) < 0) {
		puts("recv failed");
		return 0;
	}

	puts("Server reply :\n");
	for (int i = 0; i < MAX; i++) {
		printf("%d\n", integers[i]);
	}

	// close the socket
	close(sock);
	return 0;
}
