#include <stdio.h>
#include <stdlib.h>
#include "distributed.h"

clientinfo *readConfig()
{
    FILE *fptr;
    if ((fptr = fopen("./config", "r")) == NULL)
    {
        printf("Error opening file.");
        exit(1);
    }

    int slavecount;

    fscanf(fptr, "%d", &slavecount);
    printf("\n%d\n", slavecount);
    clientinfo *cinfoarray = (clientinfo *)malloc(slavecount * sizeof *cinfoarray);

    for (int i = 0; i < slavecount; i++)
    {
        int tempport;
        fscanf(fptr, "%s %d", cinfoarray[i].ipaddress, &tempport);
        cinfoarray[i].port = tempport;
    }

    fclose(fptr);
    return cinfoarray;
}

// int main()
// {
//     readConfig();
// }