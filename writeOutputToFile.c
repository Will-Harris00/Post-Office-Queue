#include <writeOutputToFile.h>

/* functions to manage writing to output file */

int writeOutputToFile(char* fileOut, char *description, char *data)
{
    FILE *fp;

    if ( (fp = fopen(fileOut, "a")) == NULL )
    {
        fprintf(stderr, "File not created\n");
        return -1;
    }
    fprintf(fp, "%s %s\n", description, data);

    fclose(fp);

    return 0;
}

/* function to write input paramaters to output input file */
void writeParametersToFile(char *fileOut, int *numSims, int *maxQueueLength, unsigned int *numServicePoints,
                           unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                           unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    char strData[20];
    snprintf(strData, 10, "%u", (*numSims));
    writeOutputToFile(fileOut, "\nNumber of Simulations:", strData);

    snprintf(strData, 10, "%d", (*maxQueueLength));
    writeOutputToFile(fileOut, "\nMax Queue Length:", strData);

    snprintf(strData, 10, "%d", (*numServicePoints));
    writeOutputToFile(fileOut, "\nNumber of Service Points:", strData);

    snprintf(strData, 10, "%d", (*closingTime));
    writeOutputToFile(fileOut, "\nTime until close:", strData);

    snprintf(strData, 10, "%d", (*averageWaitingToleranceOfCustomer));
    writeOutputToFile(fileOut, "\nAverage waiting tolerance limit:", strData);

    snprintf(strData, 10, "%d", (*averageTimeTakenToServeCustomer));
    writeOutputToFile(fileOut, "\nAverage time taken to serve customer:", strData);

    snprintf(strData, 10, "%d", (*averageNewCustomersPerInterval));
    writeOutputToFile(fileOut, "\nAverage new customers per interval:", strData);
}
