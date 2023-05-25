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
        char tempipaddress[15];
        int tempport;
        fscanf(fptr, "%s %d", tempipaddress, &tempport);
        cinfoarray[i].port = tempport;
        cinfoarray[i].ipaddress = tempipaddress;
    }

    fclose(fptr);
    return cinfoarray;
}

// int main()
// {
//     readConfig();
// }