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
    int client_sock, c, read_size;
    struct sockaddr_in slave, client;
    int message[10], i;
    int clientMessage[3];
    int *inputlist;
    float *cornersList;
    int clientStatus;

    clientinfo *configinfoarray = readConfig();

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
    // Create socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");

    for (int i = 0; i < 3; i++)
    {

        printf("%s\n", configinfoarray[i].ipaddress);
        printf("%d\n", configinfoarray[i].port);

        slave.sin_addr.s_addr = inet_addr(configinfoarray[i].ipaddress);
        slave.sin_family = AF_INET;
        slave.sin_port = htons(configinfoarray[i].port);

        // Connect to remote server
        if (connect(client_sock, (struct sockaddr *)&slave, sizeof(slave)) < 0)
        {
            printf("did not connet\n");
            return -1;
        }
        printf("Connected\n\n");
        // ACTION: SEND CORNER MATRIX INFO
        write(client_sock, &cornerMatrixInfo, 3 * sizeof(int));
        printf("corner matrix info sent.\n");

        cornersList = generateCornerMatrix(n);
        // for (size_t i = 0; i < cornerMatrixInfo[0]; i++)
        // {
        //     printf("%f ", cornersList[i]);
        // }

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
        // if (clientStatus == 2)
        // {
        //     printf("Client completed interpolation.\n");
        //     slavecountfinished += 1;
        // }
    }
    close(client_sock);
    return 0;
}