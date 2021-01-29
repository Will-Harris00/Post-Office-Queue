#include <stdio.h> /* printf */
#include <stddef.h> /* for, while, if/else, sizeof */
#include <stdlib.h> /* rand RAND_MAX */
#include <limits.h> /* datatype sizes and limits */
#include <string.h> /* string manipulation */
#include <gsl/gsl_rng.h> /* gsl library */
#include <gsl/gsl_randist.h> /* more gsl library */

/* no global variables allowed */

/* here are the structures */
struct customerStruct{
   int customerId; /* unique id assigned to each customer in order of arrival */
   int timeout; /* time before getting bored and leaving the queue */
   int waitTime; /* time before being served at service point */
};

typedef struct customerStruct CUST;

/* read these in from the command line and delete all global variables */
int numSims; /* the number of times the simulatio must be repeated */
/* add function prototype definitions here */
char fileIn[]; /* input file name containing parameters */
char fileOut[]; /* output file name where results are to be stored */

/* read these in from the input file and delete all global variables */
int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
int closingTime; /* time units until post office closes and no new customer can join the queue */
int numServicePoints; /* the number of service points at the post office */
int maxQueueLength; /* the maximum number of customers waiting in the queue */
/* can be -1 if the queue has no maximum length */


int main(){
    /* initialise an empty list to use as our queue */
    CUST *root = NULL;
}
