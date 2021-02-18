#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readInputFile.h>

/* functions to manage reading input file */

int readInputFile(char* fileIn, int *maxQueueLength, unsigned int *numServicePoints, unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                  unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    FILE *fp;
    char varName[51];

    if ( (fp = fopen(fileIn, "r")) == NULL )
    {
        fprintf(stderr, "File not openable\n");
        return -1;
    }

    while ( !feof(fp) )
    {
        if ( fscanf(fp, "%s %d\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n",
                                   varName,
                                   maxQueueLength,
                                   varName,
                                   numServicePoints,
                                   varName,
                                   closingTime,
                                   varName,
                                   averageNewCustomersPerInterval,
                                   varName,
                                   averageTimeTakenToServeCustomer,
                                   varName,
                                   averageWaitingToleranceOfCustomer) != 12 )
        {
            fprintf(stderr,"File format invalid\n");
            fclose(fp);
            return -2;
        }
    }

    fclose(fp);

    return 0;
}
