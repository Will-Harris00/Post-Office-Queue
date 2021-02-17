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
    int customerId; /* unique id assigned to each customer in order of arrival */
    int patience; /* time before getting bored and leaving the queue */
    int timeElapsed; /* time before being served at service point */
    struct customer* next;
};

typedef struct customer C;

struct servicePoint {
    int servicePointId; /* unique id assigned to each service point */
    int timeTillFinished; /* time units until customer has finished being served*/
    struct customer *serving; /* pointer to the customer currently being served */
};

typedef struct servicePoint SP;

/* The queue, front stores the customer at the front and rear
   stores the last customer */
struct queue { 
    struct customer *front, *rear; /* head, tail respectively */
};


/* function prototypes ------------------------------ */
SP *createServicePoints(int);
struct customer* newNode(int, int);

struct queue* createQueue();
void enQueue(struct queue*);
void deQueue(struct queue*);
int getCount(struct customer*);

void checkFinishedServing(SP*, int, struct queue*);
void checkPatienceLimit(struct customer**);
int checkAllSPEmpty(SP*, int);


/* no global variables allowed in final version */

/* read these in from the command line and delete all global variables */
int numSims; /* the number of times the simulatio must be repeated */
/* add function prototype definitions here */
char fileIn[1]; /* input file name containing parameters */
char fileOut[1]; /* output file name where results are to be stored */

/* read these in from the input file and delete all global variables */
int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
int closingTime; /* time units until post office closes and no new customer can join the queue */
int numServicePoints = 3; /* the number of service points at the post office */
int maxQueueLength = 5; /* the maximum number of customers waiting in the queue */
/* can be -1 if the queue has no maximum length */


/* main function ------------------------------------ */
int main()
{
    /* initialise an empty list to use as our queue */
    struct queue* q = createQueue();

    SP* servicePoints = createServicePoints(numServicePoints);
    
    /* here we generate four customers to add to the queue */
    int count = 0;
    while ( count < maxQueueLength )
    {
        enQueue(q);
        count = getCount(q->front);
    }
    printf("Count: %d\n\n", count);
    fflush(stdout);

    static int notAllEmpty = 1;

    while ( ((q->front) != NULL) || notAllEmpty )
    {   
        /* check if customers are still being served */
        notAllEmpty = !( checkAllSPEmpty(servicePoints, numServicePoints) );
        
        /* assigned waiting customer to service points and remove fufilled customers */
        checkFinishedServing(servicePoints, numServicePoints, q);
        
        /* remove all nodes that have zero patience remaining */
        checkPatienceLimit(&q->front);
    }
}


/* functions to manage the queue */

/* utility function to create a new linked list node. */
struct customer* newNode(int custId, int patience)
{
    struct customer* temp = (C*)malloc(sizeof(C));
    temp->customerId = custId;
    temp->patience = patience;
    temp->timeElapsed = 0;
    temp->next = NULL;
    return temp;
}

/* utility function to create an empty queue */
struct queue* createQueue()
{
    struct queue* q = (struct queue*)malloc(sizeof(struct queue)); 
    q->front = q->rear = NULL; 
    return q;
}

/* function to add a customer to the queue */
void enQueue(struct queue* q)
{   
    static int custId = 1;
    /* Create a new LL node */
    struct customer* temp = newNode(custId, 2); 
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
int checkAllSPEmpty(SP* servicePoints, int numServicePoints)
{   
    int allEmpty = 1;
    int x;
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


SP* createServicePoints(int numServicePoints)
{
    int x;
    SP* servicePoints = malloc(numServicePoints * sizeof *servicePoints); /* servicePoints is an array of pointers to SP structures */
    printf("Size of servicePoints pointer: %d\n",sizeof servicePoints);
    fflush(stdout);
    printf("Location of the pointer servicePoints: %p\n",&servicePoints);
    fflush(stdout);
    for ( x = 0; x < numServicePoints; x++ )
    {
        servicePoints[x].servicePointId = x + 1;
        servicePoints[x].timeTillFinished = 0;
        servicePoints[x].serving = NULL;
    }

    for ( x = 0; x < numServicePoints; x++ )
    {
        printf("SP identifier: %i\n",servicePoints[x].servicePointId);
        fflush(stdout);
    }
    return servicePoints;
}

void checkFinishedServing(SP* servicePoints, int numServicePoints, struct queue* q)
{   
    int x;
    for ( x = 0; x < numServicePoints; x++ )
    {
        if ( (servicePoints[x].serving) == NULL )
        {
            printf("Empty Service Point: %d\n", servicePoints[x].servicePointId);
            fflush(stdout);

            if ( q->front != NULL )
            {
                servicePoints[x].serving = q->front;
                servicePoints[x].timeTillFinished = 5;
                printf("Started Serving Customer: %d\n\n", (servicePoints[x].serving)->customerId);
                fflush(stdout);
                deQueue(q);
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
                C* temp; /* declare a temporary pointer to an empty customer struct */
                temp = servicePoints[x].serving; /* assign temp so we do not loose track of finished customer */
                servicePoints[x].serving = NULL; /* remove finished customer from service point */
                free(temp); /* free memory allocated to finished customer struct */

                if ( q->front != NULL )
                {
                    servicePoints[x].serving = q->front;
                    servicePoints[x].timeTillFinished = 5;
                    printf("\nFront of Queue: %d\n", q->front->customerId);
                    fflush(stdout);
                    printf("\nService Point: %d\n", servicePoints[x].servicePointId);
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
