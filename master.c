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
    int client_sock;
    struct sockaddr_in slave;
    float *cornersList;
    int clientStatus;

    clientinfo *configinfoarray = readConfigFile(argv[2]);
    int slave_count = getNConfigFile(argv[2]);

    int n = atoi(argv[1]);
    
    int slavecount = 2;
    int slavecountrecieved = 0;
    int slavecountfinished = 0;

    int nperslave = (n) / slave_count;
    if(nperslave*slave_count != (n)){
        nperslave += 1;
    }

    int chunkableperslave = nperslave / 10;
    if(chunkableperslave * 10 != nperslave){
        chunkableperslave += 1;
    }

    int finalnperslave = chunkableperslave * 10 + 1;

    int cornerLengthRow = (int)(finalnperslave / 10) + 1;
    int cornerLengthCol = (int)(finalnperslave / 10) + 1;
    int cornerMatrixInfo[3] = {
        cornerLengthRow * cornerLengthRow, // total length
        cornerLengthRow,                   // row length
        cornerLengthCol,                   // column length
    };

    cornersList = generateCornerMatrix(finalnperslave);

    struct timeval time_before, time_after;
    gettimeofday(&time_before, 0);
    for (int i = 0; i < slave_count; i++)
    {
        // Create socket
        client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sock == -1)
        {
            printf("Could not create socket");
        }
        printf("Socket created\n");

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
        for (int i = 0; i < cornerMatrixInfo[0]; i++)
		{
			printf("%f\n", cornersList[i]);
		}
    }

    gettimeofday(&time_after, 0);
    printf("completed all!");

    long seconds = time_after.tv_sec - time_before.tv_sec;
    long microseconds = time_after.tv_usec - time_before.tv_usec;
    double time_elapsed = seconds + microseconds * 1e-6;

    printf("Time elapsed: %f seconds.\n", time_elapsed);

    close(client_sock);

    return 0;
}