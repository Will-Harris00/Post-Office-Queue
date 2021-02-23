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


void unsignedTypeCasting(char* fileOut, char *description, unsigned int *data)
{
    char strData[20];
    snprintf(strData, 10, "%u", (*data));
    writeOutputToFile(fileOut, description, strData);
}


void signedTypeCasting(char* fileOut, char *description, int *data)
{
    char strData[20];
    snprintf(strData, 10, "%d", (*data));
    writeOutputToFile(fileOut, description, strData);
}


/* function to write input paramaters to output input file */
void writeParametersToFile(char *fileOut, int *numSims, int *maxQueueLength, unsigned int *numServicePoints,
                           unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                           unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    signedTypeCasting(fileOut, "\nNumber of Simulations:", numSims);

    signedTypeCasting(fileOut, "\nMax Queue Length:", maxQueueLength);

    unsignedTypeCasting(fileOut, "\nNumber of Service Points:", numServicePoints);

    unsignedTypeCasting(fileOut, "\nTime until close:", closingTime);

    unsignedTypeCasting(fileOut, "\nAverage waiting tolerance limit:", averageWaitingToleranceOfCustomer);

    unsignedTypeCasting(fileOut, "\nAverage time taken to serve customer:", averageTimeTakenToServeCustomer);

    unsignedTypeCasting(fileOut, "\nAverage new customers per interval:", averageNewCustomersPerInterval);
}


/* function to check if the output file already exists */
int stopOverwrite(char *fileOut)
{
    FILE *fp;

    if ( (fp = fopen(fileOut, "r")) != NULL )
    {
        fprintf(stderr, "Output file already exists - please choose another name\n");
        fclose(fp);
        return -1;
    }
}
