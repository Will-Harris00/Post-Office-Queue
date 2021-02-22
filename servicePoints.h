#ifndef __SERVICEPOINTS__
#define __SERVICEPOINTS__ 1

#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <randomDistributions.h>

/*----- structure definition -------------------------------------------------*/

/* A service point structure that points to the customer currently being serverd */
struct servicePoint {
    unsigned int servicePointId; /* unique id assigned to each service point */
    unsigned int timeTillFinished; /* time units until customer has finished being served*/
    struct customerNode *serving; /* pointer to the customer currently being served */
};

typedef struct servicePoint SP;


/*----- function prototypes --------------------------------------------------*/

SP *createServicePoints(unsigned int);
void checkFinishedServing(SP*, struct queue*, unsigned int, unsigned int, unsigned int*, unsigned int*);
int checkAllSPEmpty(SP*, unsigned int);

#endif
