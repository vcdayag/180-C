#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "interpolation.h"
#include "distributed.h"

#include <pthread.h>  // for threads
#include <stdlib.h>   // for malloc
#include <sys/time.h> // for gettimeofday

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    int message[10], i;
    int clientMessage[3];
    int *inputlist;

    float *cornersList;
    int clientStatus;

    clientinfo *configinfoarray = readConfig();

    for (int i = 0; i < 3; i++)
    {
        printf("%s %d\n", configinfoarray[i].ipaddress, configinfoarray[i].port);
    }

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5051);

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {

        // print the error message
        printf("bind failed. Error");
        return 1;
    }
    printf("bind done");

    int n = atoi(argv[1]);
    int cornerLengthRow = (int)(n / 10) + 1;
    int cornerLengthCol = (int)(n / 10) + 1;
    int cornerMatrixInfo[3] = {
        cornerLengthRow * cornerLengthRow, // total length
        cornerLengthRow,                   // row length
        cornerLengthCol,                   // column length
    };

    int slavecount = 2;
    int slavecountrecieved = 0;
    int slavecountfinished = 0;

    // listen to the socket
    listen(socket_desc, 3);

    while (slavecount != slavecountrecieved || slavecount != slavecountfinished)
    {

        printf("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);

        // accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

        if (client_sock < 0)
        {
            printf("accept failed");
            return 1;
        }

        printf("Connection accepted");

        printf("Waiting for client request...\n");
        if (recv(client_sock, &clientStatus, sizeof(int), 0) < 0)
        {
            printf("Error did not recieve response from client.");
            return 1;
        }
        if (clientStatus == 0)
        {
            printf("Client wants data.\n");
            printf("Sending data to client...\n");
            write(client_sock, &cornerMatrixInfo, 3 * sizeof(int));
            printf("corner matrix info sent.\n");

            cornersList = generateCornerMatrix(n);
            for (size_t i = 0; i < cornerMatrixInfo[0]; i++)
            {
                printf("%f ", cornersList[i]);
            }

            write(client_sock, cornersList, cornerMatrixInfo[0] * sizeof(float *));
            printf("corner array sent.\n");

            if (recv(client_sock, &clientStatus, sizeof(int), 0) < 0)
            {
                printf("Error did not recieve response from client.");
                return 1;
            }

            if (clientStatus == 1)
            {
                printf("Client recieved data.\n");
                slavecountrecieved += 1;
            }
            else
            {
                printf("Error wrong status: %d", clientStatus);
            }
        }
        else if (clientStatus == 2)
        {
            printf("Client completed interpolation.\n");
            slavecountfinished += 1;
        }
    }

    close(socket_desc);
    return 0;
}
