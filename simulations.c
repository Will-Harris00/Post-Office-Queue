#include <simulations.h>

void runSimulations(char *fileOut, int *numSims, int *maxQueueLength, unsigned int *numServicePoints,
                    unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                    unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    unsigned int existsGSL = 0;
    gsl_rng            *r;

    /* int n; */
    /* n = chooseDistribution(1,1,1, &r, &existsGSL); *//* option 1: Uniform/Flat */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,2, &r, &existsGSL); *//* option 2: Normal/Gaussian */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,3, &r, &existsGSL); *//* option 3: Poisson */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,4, &r, &existsGSL); *//* option 4: Gamma */
    /* printf("%u\n\n", (unsigned int)n); */

    /*
    int i;
    for ( i = 0; i < 5; i++ )
    {
        n = chooseDistribution(2,2,3, &r, &existsGSL);
        printf("%d\n",n);
        fflush(stdout);
    }
    exit(0); *//* test random number generation with GSL */
    

    unsigned int s = 1; /* number of simulations*/
    unsigned int tolerance; /* random customer waiting tolerance limit */
    unsigned int newCustomers; /* random number of new customers */

    /* totals across all simulations */
    unsigned int totalFulfilled = 0; /* fulfilled customer are counted at the time they start being served */
    unsigned int totalUnfulfilled = 0;
    unsigned int totalTimedOut = 0;
    unsigned int totalWaitTime = 0;
    unsigned int totalTimeAfterClose = 0;

    while ( s <= (*numSims) )
    {
        printf("\n\n\nSimulation number: %u\n", s);
        fflush(stdout);
        unsigned int custId = 1;
        /* simulation statistics */
        unsigned int fulfilled = 0; /* count fulfilled customers */
        unsigned int unfulfilled = 0; /* count unfulfilled customers*/
        unsigned int timedOut = 0; /* count timed-out customers */
        unsigned int combinedWaitTime = 0; /* count collective time spend waiting by fulfilled customers */

        /* initialise an empty list to use as our queue */
        struct queue* q = createQueue();

        SP* servicePoints = createServicePoints((*numServicePoints));
        
        unsigned int timeUnits = 0;
        unsigned int numSPInUse = 0;
        unsigned int count = 0;
        while ( timeUnits < (*closingTime) || (q->front) != NULL || numSPInUse > 0 )
        {
            printf("Time units: %u\n", timeUnits);
            fflush(stdout);
            count = getCount(q->front);
            printf("Count: %u\n\n", count);
            fflush(stdout);
            if ( (*numSims) == 1 ) /* write current time unit to output file */
                unsignedTypeCasting(fileOut, "\nTime units:", &timeUnits);

            if ( (q->front) != NULL || numSPInUse > 0 )
            {
                /* assign waiting customer to service points and remove fulfilled customers */
                checkFinishedServing(servicePoints, q, (*numServicePoints), (*averageTimeTakenToServeCustomer), 
                                     &fulfilled, &combinedWaitTime, &r, &existsGSL);

                /* remove all nodes that have zero patience remaining */
                checkPatienceLimit(&q->front, &timedOut);

                /* check if customers are still being served */
                numSPInUse = countSPInUse(servicePoints, (*numServicePoints));
            }

            if ( (*numSims) == 1 )
            {
                /* write current number of customer being served and number in queue to output file */
                unsignedTypeCasting(fileOut, "Number of customers being served:", &numSPInUse);
                unsignedTypeCasting(fileOut, "Number of customers in queue:", &count);
            }

            if ( timeUnits < (*closingTime) )
            {
                unsigned int j;
                newCustomers = chooseDistribution((*averageNewCustomersPerInterval),3,3, &r, &existsGSL); /* Poisson Distribution: mean avgServingTime, standard deviation 3 */
                printf("Number of new customers: %u\n", newCustomers);
                fflush(stdout);
                for ( j = 0; j < newCustomers; j++ )
                {
                    if ( count < (*maxQueueLength) || (*maxQueueLength) == -1 )
                    {
                        tolerance = chooseDistribution((*averageWaitingToleranceOfCustomer),3,3, &r, &existsGSL); /* Poisson Distribution: mean avgServingTime, standard deviation 3 */
                        enQueue(q, tolerance, &custId);
                    }
                    else
                        ++(unfulfilled); /* increment unfulfilled customers when they attempt to join a full queue */
                    count = getCount(q->front); /* we initialised count to zero so no need to get count on first iteration */
                }
            }

            if ( (*numSims) == 1 )
            {
                /* write current number of fulfilled, unfulfilled and timed-out customers */
                unsignedTypeCasting(fileOut, "Current Number of fulfilled customers:", &fulfilled);
                unsignedTypeCasting(fileOut, "Current Number of unfulfilled customers:", &unfulfilled);
                unsignedTypeCasting(fileOut, "Current Number of timed-out customers:", &timedOut);
                if ( timeUnits == (*closingTime) ) /* add marker to the file showing closing time */
                    unsignedTypeCasting(fileOut, "\n\nClosing time reached:", closingTime);
            }

            timeUnits++;
        }

        /* count = getCount(q->front); */
        printf("\n\nFinal count: %u\n", count);
        printf("End time: %u\n\n", timeUnits);
        free(servicePoints);
        free(q);

        unsigned int timeAfterClose;
        timeAfterClose = timeUnits-(*closingTime);

        if ( (*numSims) == 1 )
            unsignedTypeCasting(fileOut, "Time after close all customers leave:", &timeAfterClose);

        /* calculate seconds from closing time till finished serving all customers in queue */
        printf("Fulfilled customers: %u\n", fulfilled); /* fulfilled customer are counted at the time they start being served */
        printf("Unfulfilled customers: %u\n", unfulfilled);
        printf("Timed-out customers: %u\n", timedOut);
        printf("Combined time spent waiting by fufilled customers: %u\n", combinedWaitTime);
        printf("Time after closing finished serving: %u\n", timeAfterClose);
        fflush(stdout);

        totalTimedOut += timedOut;
        totalUnfulfilled += unfulfilled;
        totalFulfilled += fulfilled; /* fulfilled customer are counted at the time they start being served */
        totalWaitTime += combinedWaitTime;
        totalTimeAfterClose += timeAfterClose;
        s++;
    }

    if ( (*numSims) != 1 )
    {
        printf("\nTotal fulfilled customers: %u\n", totalFulfilled); /* fulfilled customer are counted at the time they start being served */
        printf("Total unfulfilled customers: %u\n", totalUnfulfilled);
        printf("Total timed-out customers: %u\n", totalTimedOut);
        printf("Total combined time spent waiting by fufilled customers: %u\n", totalWaitTime);
        printf("Total combined time after closing until customers leave: %u\n", totalTimeAfterClose);
        fflush(stdout);
    }

    /* calculate averages for multiple simulations */
    if ( (*numSims) != 1 )
    {
        float avgFulfilled = (float)totalFulfilled / (float)(*numSims);
        float avgUnfulfilled = (float)totalUnfulfilled / (float)(*numSims);
        float avgTimedOut = (float)totalTimedOut / (float)(*numSims);
        printf("\nAverage number of fulfilled customers: %f\n", avgFulfilled);
        floatTypeCasting(fileOut, "\nAverage number of fulfilled customers:", &avgFulfilled);
        printf("Average number of unfulfilled customers: %f\n", avgUnfulfilled);
        floatTypeCasting(fileOut, "Average number of unfulfilled customers:", &avgUnfulfilled);
        printf("Average number of timed-out customers: %f\n", avgTimedOut);
        floatTypeCasting(fileOut, "Average number of timed-out customers:", &avgTimedOut);
    }


    float avgWaitFulfilled = (float)totalWaitTime / (float)totalFulfilled;
    printf("Average wait time for fulfilled customers: %f\n", avgWaitFulfilled);
    floatTypeCasting(fileOut, "Average wait for fulfilled customers:", &avgWaitFulfilled);

    /* calculate average time after closing for multiple simulations */
    if ( (*numSims) != 1 )
    {
        float avgTimeAfterClose = (float)totalTimeAfterClose / (float)(*numSims);
        printf("Average time after close until customers leave: %f\n", avgTimeAfterClose);
        floatTypeCasting(fileOut, "Average time after close until customers leave:", &avgTimeAfterClose);
    }

    gsl_rng_free(r); /* free the memory allocated to GSL random number generator */
}
