// Server code in C to sort the array
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pthread.h>  // for threads
#include <stdlib.h>   // for malloc
#include <sys/time.h> // for gettimeofday
#include "interpolation.h"

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    int message[10], i;
    int clientMessage[3];
    int *inputlist;

    float *cornersList;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5600);

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {

        // print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // listen to the socket
    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    puts("Connection accepted");

    int n = 11;
    int cornerLengthRow = (int)(n / 10) + 1;
    int cornerLengthCol = (int)(n / 10) + 1;
    int cornerMatrixInfo[3] = {
        cornerLengthRow * cornerLengthRow, // total length
        cornerLengthRow,                   // row length
        cornerLengthCol,                   // column length
    };

    write(client_sock, &cornerMatrixInfo, 3 * sizeof(int));
    recv(client_sock, &clientMessage, 3 * sizeof(int), 0);

    cornersList = generateCornerMatrix(n);
    for (size_t i = 0; i < cornerMatrixInfo[0]; i++)
    {
        printf("%f ", cornersList[i]);
    }

    write(client_sock, cornersList, cornerMatrixInfo[0] * sizeof(float *));
    recv(client_sock, &clientMessage, 3 * sizeof(int), 0);

    if (read_size == 0)
    {
        puts("Client disconnected");
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}