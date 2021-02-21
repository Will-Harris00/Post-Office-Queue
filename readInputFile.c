#include <readInputFile.h>

/* functions to manage reading input file */

int readInputFile(char* fileIn, int *maxQueueLength, unsigned int *numServicePoints,
                  unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
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
    printf("Successfully loaded parameters\n");

    fclose(fp);
    if ( validateParameters(maxQueueLength, numServicePoints, closingTime, averageNewCustomersPerInterval, averageNewCustomersPerInterval) )
        exit(-5);

    return 0;
}

int validateParameters(int *maxQueueLength, unsigned int *numServicePoints, unsigned int *closingTime,
                        unsigned int *averageWaitingToleranceOfCustomer, unsigned int *averageNewCustomersPerInterval)
{
    if ( *maxQueueLength < -1 )
    {
        fprintf(stderr, "Invalid max queue length\n");
        return -1;
    }
    if ( *numServicePoints < 1 )
    {
        fprintf(stderr, "Invalid number of service points\n");
        return -1;
    }
    if ( *closingTime < 1 )
    {
        fprintf(stderr, "Invalid closing time\n");
        return -1;
    }
    if ( *averageNewCustomersPerInterval < 1 )
    {
        fprintf(stderr, "Invalid number of new customers per interval\n");
        return -1;
    }
    if ( *averageWaitingToleranceOfCustomer < 1 )
    {
        fprintf(stderr, "Invalid customer waiting tolerance\n");
        return -1;
    }

    return 0;
}
