#include <servicePoints.h>

/* functions to manage the service points */

/* function to create service points array */
SP* createServicePoints(unsigned int numSP)
{
    unsigned int x;
    SP* servicePoints; /* servicePoints is an array of pointers to SP structures */
    if ( !( servicePoints = (SP *)malloc(numSP * (sizeof(*servicePoints))) ) ) /* check memory assignment */
    {
        fprintf(stderr, "Out of memory\n");
        exit(-1);
    }
    printf("Size of servicePoints pointer: %d\n", sizeof servicePoints);
    fflush(stdout);
    printf("Location of the pointer servicePoints: %p\n", &servicePoints);
    fflush(stdout);
    for ( x = 0; x < numSP; x++ )
    {
        servicePoints[x].servicePointId = x + 1;
        servicePoints[x].timeTillFinished = 0;
        servicePoints[x].serving = NULL;
    }

    for ( x = 0; x < numSP; x++ )
    {
        printf("SP identifier: %u\n",servicePoints[x].servicePointId);
        fflush(stdout);
    }
    return servicePoints;
}


/* function to remove customer that have finished being served and assigned new customers from queue */
void checkFinishedServing(SP* servicePoints, struct queue* q, unsigned int numSP, unsigned int avgServingTime, 
                          unsigned int *fulfilled, unsigned int *combinedWaitTime, gsl_rng **r, unsigned int *existsGSL)
{
    unsigned int x;
    unsigned int servingTime;
    for ( x = 0; x < numSP; x++ )
    {
        printf("\nService Point: %u\n", servicePoints[x].servicePointId);
        fflush(stdout);
        if ( (servicePoints[x].serving) == NULL )
        {
            printf("Empty Service Point: %u\n", servicePoints[x].servicePointId);
            fflush(stdout);

            if ( q->front != NULL )
            {
                servicePoints[x].serving = q->front;
                servingTime = chooseDistribution(avgServingTime,3,3, r, existsGSL); /* Poisson Distribution: mean avgServingTime, standard deviation 3 */
                servicePoints[x].timeTillFinished = servingTime;

                printf("Started Serving Customer: %u\n\n", (servicePoints[x].serving)->customerId);
                fflush(stdout);

                ++(*fulfilled); /* increment fulfilled customers when finished being served */
                unsigned int customerWaitTime = (servicePoints[x].serving)->timeElapsed - (servicePoints[x].serving)->patience;
                *combinedWaitTime += customerWaitTime; /* collate time spent in queue for fulfilled customer */
                deQueue(q);
                continue; /* ensures waiting tolerance limit is not decremented for newly served customer */
            }
            continue;
        }

        if ( (servicePoints[x].serving) != NULL )
        {
            --(servicePoints[x].timeTillFinished); /* Decrement time till finished serving counter */
            printf("Time Remaining: %u\n", servicePoints[x].timeTillFinished);
            fflush(stdout);

            if ( (servicePoints[x].timeTillFinished) == 0 )
            {
                CN* tmp; /* declare a temporary pointer to an empty customer struct */
                tmp = servicePoints[x].serving; /* assign temp so we do not loose track of finished customer */
                servicePoints[x].serving = NULL; /* remove finished customer from service point */
                /* free(tmp); */ /* free memory allocated to finished customer struct */

                if ( q->front != NULL )
                {
                    servicePoints[x].serving = q->front;
                    servingTime = chooseDistribution(avgServingTime,3,3, r, existsGSL); /* Poisson Distribution: mean avgServingTime, standard deviation 3 */
                    servicePoints[x].timeTillFinished = servingTime;

                    printf("\nFront of Queue: %u\n", q->front->customerId);
                    fflush(stdout);
                    printf("Started Serving Customer: %u\n\n", (servicePoints[x].serving)->customerId);
                    fflush(stdout);

                    ++(*fulfilled); /* increment fulfilled customers when finished being served */
                    unsigned int customerWaitTime = (servicePoints[x].serving)->timeElapsed - (servicePoints[x].serving)->patience;
                    *combinedWaitTime += customerWaitTime; /* collate time spent in queue for fulfilled customer */
                    deQueue(q);
                }
            }
        }
    }
}


/* function to count how many service points are serving customers */
int countSPInUse(SP* servicePoints, unsigned int numServicePoints)
{
    unsigned int numSPInUse = 0;
    unsigned int x;
    for ( x = 0; x < numServicePoints; x++ )
    {
        if ( (servicePoints[x].serving) != NULL )
        {
            numSPInUse++;
        }
    }
    return numSPInUse;
}
