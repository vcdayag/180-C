#ifndef DISTRIBUTEDH /* This is an "include guard" */
#define DISTRIBUTEDH /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/

/*
    0 - Number of values in the array
    1 - row size of the MATRIX
    2 - column size of the MATRIX
*/
extern int cornerMatrxInfo[3];

/*
    0 - request to get data
    1 - data recieved
    2 - completed interpolation
*/

extern int clientStatus;

typedef struct
{
    char *ipaddress;
    int port;
} clientinfo;

clientinfo *readConfig();

#endif /* DISTRIBUTEDH */