#include <stdio.h> /* printf */
#include <stddef.h> /* for, while, if/else, sizeof */
#include <stdlib.h> /* rand RAND_MAX */
#include <limits.h> /* datatype sizes and limits */
#include <string.h> /* string manipulation */
#include <gsl/gsl_rng.h> /* gsl library */
#include <gsl/gsl_randist.h> /* more gsl library */

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

/* A linked list node to store a customer queue entry */
struct customer {
    unsigned int customerId; /* unique id assigned to each customer in order of arrival */
    unsigned int patience; /* time before getting bored and leaving the queue */
    unsigned int timeElapsed; /* time before being served at service point */
    struct customer* next;
};

typedef struct customer C;

struct servicePoint {
    unsigned int servicePointId; /* unique id assigned to each service point */
    unsigned int timeTillFinished; /* time units until customer has finished being served*/
    struct customer *serving; /* pointer to the customer currently being served */
};

typedef struct servicePoint SP;

/* The queue, front stores the customer at the front and rear
   stores the last customer */
struct queue { 
    struct customer *front, *rear; /* head, tail respectively */
};


/* function prototypes ------------------------------ */
SP *createServicePoints(unsigned int);
struct customer* newNode(unsigned int, unsigned int);

struct queue* createQueue();
void enQueue(struct queue*, int);
void deQueue(struct queue*);
int getCount(struct customer*);

void checkFinishedServing(SP*, struct queue*, unsigned int, unsigned int);
void checkPatienceLimit(struct customer**);
int checkAllSPEmpty(SP*, unsigned int);

int readInputFile(int *, unsigned int *, unsigned int *, unsigned int *, unsigned int *, unsigned int *);


/* no global variables allowed in final version */

/* read these in from the command line and delete all global variables */
unsigned int numSims; /* the number of times the simulation must be repeated */
/* add function prototype definitions here */
char fileIn[1]; /* input file name containing parameters */
char fileOut[1]; /* output file name where results are to be stored */


/* main function ------------------------------------ */
int main()
{   
    /* read these in from the input file */
    unsigned int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
    unsigned int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
    unsigned int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
    unsigned int closingTime; /* time units until post office closes and no new customer can join the queue */
    unsigned int numServicePoints; /* the number of service points at the post office */
    int maxQueueLength; /* the maximum number of customers waiting in the queue */
    /* can be -1 if the queue has no maximum length */

    if ( readInputFile(&maxQueueLength, &numServicePoints, &closingTime, &averageWaitingToleranceOfCustomer, 
                  &averageTimeTakenToServeCustomer, &averageNewCustomersPerInterval) )
                  exit(-1);

    printf("maxQueueLength: %d\n", maxQueueLength);
    printf("numServicePoints: %u\n", numServicePoints);
    printf("closingTime: value: %u\n", closingTime);
    printf("averageNewCustomersPerInterval: %u\n", averageNewCustomersPerInterval);
    printf("averageTimeTakenToServeCustomer: %u\n", averageTimeTakenToServeCustomer);
    printf("averageWaitingToleranceOfCustomer: %u\n", averageWaitingToleranceOfCustomer);

    /* initialise an empty list to use as our queue */
    struct queue* q = createQueue();

    SP* servicePoints = createServicePoints(numServicePoints);
    
    unsigned int timeUnits = 0;
    unsigned int notAllEmpty = 0;
    unsigned int count = 0;
    while ( timeUnits < closingTime || (q->front) != NULL || notAllEmpty)
    {
        printf("Time units: %d\n", timeUnits);

        if ( (q->front) != NULL || notAllEmpty )
        {
            /* assigned waiting customer to service points and remove fufilled customers */
            checkFinishedServing(servicePoints, q, numServicePoints, averageTimeTakenToServeCustomer);
            
            /* remove all nodes that have zero patience remaining */
            checkPatienceLimit(&q->front);

            /* check if customers are still being served */
            notAllEmpty = !( checkAllSPEmpty(servicePoints, numServicePoints) );
        }

        if ( timeUnits < closingTime )
        {
            unsigned int j;
            for ( j = 0; j < averageNewCustomersPerInterval; j++ )
            {   
                count = getCount(q->front);
                if ( count < maxQueueLength )
                {
                    enQueue(q, averageWaitingToleranceOfCustomer);
                }
            }
            printf("Count: %d\n\n", count);
            fflush(stdout);
        }

        timeUnits++;
    }
    
    free(servicePoints);
    free(q);
}


/* functions to manage the queue */

/* utility function to create a new linked list node. */
struct customer* newNode(unsigned int custId, unsigned int patience)
{   
    struct customer* temp;
    if ( !( temp = (C *)malloc(sizeof(C)) ) ) /* check memory assignment */
    {
        printf("Out of memory\n");
        exit(1);
    }

    temp->customerId = custId;
    temp->patience = patience;
    temp->timeElapsed = 0;
    temp->next = NULL;
    return temp;
}

/* utility function to create an empty queue */
struct queue* createQueue()
{
    struct queue* q;
    if ( !( q = (struct queue*)malloc(sizeof(struct queue)) ) ) /* check memory assignment */
    {
        printf("Out of memory\n");
        exit(1);
    }
    q->front = q->rear = NULL; 
    return q;
}

/* function to add a customer to the queue */
void enQueue(struct queue* q, int patience)
{   
    static unsigned int custId = 1;
    /* Create a new LL node */
    struct customer* temp = newNode(custId, patience); 
    custId++;

    /* If queue is empty, then new node is front and rear both */
    if ( q->rear == NULL )
    {
        q->front = q->rear = temp;
        return;
    } 

    /* Add the new node at the end of queue and change rear */
    q->rear->next = temp;
    q->rear = temp;
}

/* function to remove a customer from the queue */
void deQueue(struct queue* q)
{
    /* If queue is empty, return NULL. */
    if ( q->front == NULL ) 
        return; 

    /* Store previous front and move front one node ahead */
    struct customer* temp = q->front;

    q->front = q->front->next;

    /* If front becomes NULL, then change rear also as NULL */
    if ( q->front == NULL )
        q->rear = NULL;

    free(temp);
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

/* function to count number of nodes in queue */
int getCount(struct customer* head)
{ 
    /* Base case */
    if ( head == NULL )
        return 0;
  
    /* count is 1 + count of remaining list */
    return 1 + getCount(head->next);
} 


SP* createServicePoints(unsigned int numSP)
{
    unsigned int x;
    SP* servicePoints = malloc(numSP * sizeof *servicePoints); /* servicePoints is an array of pointers to SP structures */
    printf("Size of servicePoints pointer: %d\n",sizeof servicePoints);
    fflush(stdout);
    printf("Location of the pointer servicePoints: %p\n",&servicePoints);
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

void checkFinishedServing(SP* servicePoints, struct queue* q, unsigned int numSP, unsigned int servingTime)
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
                C* tmp; /* declare a temporary pointer to an empty customer struct */
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
                    deQueue(q);
                }
            }
        }
    }
}


void checkPatienceLimit(struct customer** head_ref)
{   
    /* Store rear (head) node */
    struct customer *temp = *head_ref, *prev;
 
    /* If the rear (head) node itself has zero patience remaining */
    while ( temp != NULL && temp->patience == 0 )
    {
        printf("Removed customer: %d\n", temp->customerId);
        fflush(stdout);

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


/* functions to manage reading input file */

int readInputFile(int *maxQueueLength, unsigned int *numServicePoints, unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                  unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    FILE *fp;
    char varName[51];

    /* short int is not big enough so would overwrite
       the start of name and cause to terminate early */

    fp = fopen("input.txt", "r");
    if ( fp==NULL )
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