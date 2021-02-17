#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp;
    char varName[51];
    unsigned int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
    unsigned int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
    unsigned int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
    unsigned int closingTime; /* time units until post office closes and no new customer can join the queue */
    unsigned int numServicePoints; /* the number of service points at the post office */
    int maxQueueLength; /* the maximum number of customers waiting in the queue */
    /* can be -1 if the queue has no maximum length */

    /* short int is not big enough so would overwrite
       the start of name and cause to terminate early */

    if ( (fp = fopen("testInput.txt", "r")) == NULL )
    {
        fprintf(stderr, "File not openable\n");
        return -1;
    }

    while ( !feof(fp) )
    {
        if ( fscanf(fp, "%s %d\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n",
                                   varName,
                                   &maxQueueLength,
                                   varName,
                                   &numServicePoints,
                                   varName,
                                   &closingTime,
                                   varName,
                                   &averageNewCustomersPerInterval,
                                   varName,
                                   &averageTimeTakenToServeCustomer,
                                   varName,
                                   &averageWaitingToleranceOfCustomer) != 12 )
        {
            fprintf(stderr,"File format invalid\n");
            fclose(fp);
            return -2;
        }
    }
    printf("maxQueueLength: %d\n", maxQueueLength);
    printf("numServicePoints: %u\n", numServicePoints);
    printf("closingTime: value: %u\n", closingTime);
    printf("averageNewCustomersPerInterval: %u\n", averageNewCustomersPerInterval);
    printf("averageTimeTakenToServeCustomer: %u\n", averageTimeTakenToServeCustomer);
    printf("averageWaitingToleranceOfCustomer: %u\n", averageWaitingToleranceOfCustomer);

    fclose(fp);

    return 0;
}
