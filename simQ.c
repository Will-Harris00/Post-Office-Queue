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


/*----- structure definitions  ---------------------- */

/* A service point structure that points to the customer currently being serverd */
struct servicePoint {
    unsigned int servicePointId; /* unique id assigned to each service point */
    unsigned int timeTillFinished; /* time units until customer has finished being served*/
    struct customerNode *serving; /* pointer to the customer currently being served */
};

typedef struct servicePoint SP;


/* function prototypes ------------------------------ */
SP *createServicePoints(unsigned int);

void checkFinishedServing(SP*, struct queue*, unsigned int, unsigned int, unsigned int*, unsigned int*);
void checkPatienceLimit(struct customerNode**, unsigned int*);
int checkAllSPEmpty(SP*, unsigned int);


/* main function ------------------------------------ */
int main(int argc, char **argv)
{
    unsigned int existsGSL = 0;
    unsigned int n;
    gsl_rng            *r;
    n = chooseDistribution(1,1,2, &r, &existsGSL);
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,2, &r, &existsGSL);
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,3, &r, &existsGSL);
    printf("%u\n", (unsigned int)n);
    n = chooseDistribution(1,1,4, &r, &existsGSL);
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

    /* simulation statistics */
    unsigned int timedOut = 0;
    unsigned int unfulfilled = 0; /* count unfulfilled customers*/
    unsigned int fulfilled = 0; /* count fulfilled customers */
    unsigned int totalWaitTime; /* count collective wait time elapsed of fulfilled customers */

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
                    enQueue(q, averageWaitingToleranceOfCustomer);
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
    gsl_rng_free(r); /* free the memory allocated to GSL random number generator */

    /* calculate seconds from closing time till finished serving all customers in queue */
    printf("Time after closing finished serving: %u\n", (timeUnits-closingTime));
    printf("Timed-out customers: %u\n", timedOut);
    printf("Unfulfilled customers: %u\n", unfulfilled);
    printf("Fulfilled customers: %u\n", fulfilled);
    printf("Total time spent waiting by fufilled customers: %u\n", totalWaitTime);
}


/* function to check all service points are empty */
int checkAllSPEmpty(SP* servicePoints, unsigned int numServicePoints)
{   
    unsigned int allEmpty = 1;
    unsigned int x;
    for ( x = 0; x < numServicePoints; x++ )
    {
        if ( (servicePoints[x].serving) != NULL )
        {
            allEmpty = 0;
        }
    }
    return allEmpty;
}


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
        printf("SP identifier: %i\n",servicePoints[x].servicePointId);
        fflush(stdout);
    }
    return servicePoints;
}

void checkFinishedServing(SP* servicePoints, struct queue* q, unsigned int numSP, unsigned int servingTime, unsigned int *fulfilled, unsigned int *totalWaitTime)
{   
    unsigned int x;
    for ( x = 0; x < numSP; x++ )
    {   
        printf("\nService Point: %d\n", servicePoints[x].servicePointId);
        fflush(stdout);
        if ( (servicePoints[x].serving) == NULL )
        {
            printf("Empty Service Point: %d\n", servicePoints[x].servicePointId);
            fflush(stdout);

            if ( q->front != NULL )
            {
                servicePoints[x].serving = q->front;
                servicePoints[x].timeTillFinished = servingTime;
                printf("Started Serving Customer: %d\n\n", (servicePoints[x].serving)->customerId);
                fflush(stdout);

                ++(*fulfilled); /* increment fulfilled customers when finished being served */
                unsigned int customerWaitTime = (servicePoints[x].serving)->timeElapsed - (servicePoints[x].serving)->patience;
                *totalWaitTime += customerWaitTime; /* collate time spent in queue for fulfilled customer */
                deQueue(q);
                continue; /* ensures waiting tolerance limit is not decremented for newly served customer */
            }
            continue;
        }

        if ( (servicePoints[x].serving) != NULL )
        {
            --(servicePoints[x].timeTillFinished); /* Decrement time till finished serving counter */
            printf("Time Remaining: %d\n", servicePoints[x].timeTillFinished);
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
                    servicePoints[x].timeTillFinished = servingTime;
                    printf("\nFront of Queue: %d\n", q->front->customerId);
                    fflush(stdout);
                    printf("Started Serving Customer: %d\n\n", (servicePoints[x].serving)->customerId);
                    fflush(stdout);

                    ++(*fulfilled); /* increment fulfilled customers when finished being served */
                    unsigned int customerWaitTime = (servicePoints[x].serving)->timeElapsed - (servicePoints[x].serving)->patience;
                    *totalWaitTime += customerWaitTime; /* collate time spent in queue for fulfilled customer */
                    deQueue(q);
                }
            }
        }
    }
}


void checkPatienceLimit(struct customerNode** head_ref, unsigned int *timedOut)
{   
    /* Store rear (head) node */
    struct customerNode *temp = *head_ref, *prev;
 
    /* If the rear (head) node itself has zero patience remaining */
    while ( temp != NULL && temp->patience == 0 )
    {
        printf("Removed customer: %d\n", temp->customerId);
        fflush(stdout);
        ++(*timedOut);

        *head_ref = temp->next; /* Changed head */
        free(temp); /* free old head */
        temp = *head_ref;
    }

    /* Delete occurrences other than head */
    while ( temp != NULL ) 
    {   
        /* Search for nodes to be deleted, keep track of the
        previous node as we need to change 'prev->next' */
        while ( temp != NULL && temp->patience != 0 )
        {   
            --(temp->patience); /* decrement waiting tolerance limit */
            prev = temp;
            temp = temp->next;
        }
    
        /* If no node with zero patience is present in linked list queue */
        if ( temp == NULL )
            return;
    
        /* Unlink the node from linked list */
        printf("Removed customer: %d\n", temp->customerId);
        fflush(stdout);
        prev->next = temp->next;
        free(temp); /* Free memory */

        /* Update temp for next iteration of outer loop */
        temp = prev->next;
    }
}
