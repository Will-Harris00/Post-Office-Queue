#include <simQ.h>

/*
At each time interval the following happens or may happen, in this order:

1. Zero or more fulfilled customers have finished being served and leave the system.

2. A service point that is not serving a customer will start to serve the next available customer,
and that customer will leave the queue.

3. If a customer in the queue has reached their waiting tolerance limit, then they leave the queue
and become a timed-out customer.

4. Zero or more customers arrive into the system, and either join the back of the queue, or, if the
queue is full, leave the system as an unfulfilled customer.
*/


/* main function ------------------------------------ */
int main(int argc, char **argv)
{
    unsigned int s = 1;
    unsigned int existsGSL = 0;
    unsigned int n;
    gsl_rng            *r;
    n = chooseDistribution(1,1,1, &r, &existsGSL); /* option 1: Uniform/Flat */ 
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,2, &r, &existsGSL); /* option 2: Normal/Gaussian */
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,3, &r, &existsGSL); /* option 3: Poisson */
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,4, &r, &existsGSL); /* option 4: Gamma */
    printf("%u\n\n", (unsigned int)n);

    if ( argc != 4 )
    {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(-2);
    }


    /* read these in from the command line */
    char* fileIn = argv[1]; /* input file name containing parameters */
    int numSims = atoi(argv[2]); /* the number of times the simulation must be repeated */
    char* fileOut = argv[3]; /* output file name where results are to be stored */

    if ( numSims < 1 )
    {
        fprintf(stderr, "Invalid number of simulations\n");
        exit(-4);
    }

    printf("fileIn: %s\n", fileIn);
    printf("numSims: %d\n", numSims);
    printf("fileOut: %s\n\n", fileOut);

    /* read these in from the input file */
    unsigned int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
    unsigned int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
    unsigned int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
    unsigned int closingTime; /* time units until post office closes and no new customer can join the queue */
    unsigned int numServicePoints; /* the number of service points at the post office */
    int maxQueueLength; /* the maximum number of customers waiting in the queue */
    /* can be -1 if the queue has no maximum length */

    if ( readInputFile(fileIn, &maxQueueLength, &numServicePoints, &closingTime, &averageWaitingToleranceOfCustomer, 
                  &averageTimeTakenToServeCustomer, &averageNewCustomersPerInterval) )
                  exit(-3);

    printf("maxQueueLength: %d\n", maxQueueLength);
    printf("numServicePoints: %u\n", numServicePoints);
    printf("closingTime: value: %u\n", closingTime);
    printf("averageNewCustomersPerInterval: %u\n", averageNewCustomersPerInterval);
    printf("averageTimeTakenToServeCustomer: %u\n", averageTimeTakenToServeCustomer);
    printf("averageWaitingToleranceOfCustomer: %u\n\n", averageWaitingToleranceOfCustomer);

    while ( s <= numSims)
    {
        printf("\n\n\nSimulation number: %u\n", s);
        fflush(stdout);
        unsigned int custId = 1;
        /* simulation statistics */
        unsigned int timedOut = 0;
        unsigned int unfulfilled = 0; /* count unfulfilled customers*/
        unsigned int fulfilled = 0; /* count fulfilled customers */
        unsigned int totalWaitTime = 0; /* count collective wait time elapsed of fulfilled customers */

        /* initialise an empty list to use as our queue */
        struct queue* q = createQueue();

        SP* servicePoints = createServicePoints(numServicePoints);
        
        unsigned int timeUnits = 0;
        unsigned int notAllEmpty = 0;
        unsigned int count = 0;
        while ( timeUnits < closingTime || (q->front) != NULL || notAllEmpty)
        {
            printf("Time units: %d\n", timeUnits);
            count = getCount(q->front);
            printf("Count: %u\n\n", count);
            fflush(stdout);

            if ( (q->front) != NULL || notAllEmpty )
            {
                /* assigned waiting customer to service points and remove fulfilled customers */
                checkFinishedServing(servicePoints, q, numServicePoints, averageTimeTakenToServeCustomer, &fulfilled, &totalWaitTime);
                
                /* remove all nodes that have zero patience remaining */
                checkPatienceLimit(&q->front, &timedOut);

                /* check if customers are still being served */
                notAllEmpty = !( checkAllSPEmpty(servicePoints, numServicePoints) );
            }

            if ( timeUnits < closingTime )
            {
                unsigned int j;
                for ( j = 0; j < averageNewCustomersPerInterval; j++ )
                {
                    if ( count < maxQueueLength || maxQueueLength == -1 )
                        enQueue(q, averageWaitingToleranceOfCustomer, &custId);
                    else
                        ++(unfulfilled); /* increment unfulfilled customers when they attempt to join a full queue */
                    count = getCount(q->front); /* we initialised count to zero so no need to get count on first iteration */
                }
            }

            timeUnits++;
        }

        /* count = getCount(q->front); */
        printf("\n\nFinal count: %u\n", count);
        printf("End time: %u\n\n", timeUnits);
        free(servicePoints);
        free(q);

        /* calculate seconds from closing time till finished serving all customers in queue */
        printf("Time after closing finished serving: %u\n", (timeUnits-closingTime));
        printf("Timed-out customers: %u\n", timedOut);
        printf("Unfulfilled customers: %u\n", unfulfilled);
        printf("Fulfilled customers: %u\n", fulfilled);
        printf("Total time spent waiting by fufilled customers: %u\n", totalWaitTime);

        s++;
    }
    gsl_rng_free(r); /* free the memory allocated to GSL random number generator */

    return 0;
}
