#ifndef __QUEUE__
#define __QUEUE__ 1

#include <stdio.h>
#include <stdlib.h>

/*----- structure definition -------------------------------------------------*/

/* A linked list node to store a customer queue entry */
struct customerNode {
    unsigned int customerId; /* unique id assigned to each customer in order of arrival */
    unsigned int patience; /* time before getting bored and leaving the queue */
    unsigned int timeElapsed; /* time before being served at service point */
    struct customerNode* next;
};

typedef struct customerNode CN;

/* The queue, front stores the customer at the front and rear stores the last customer */
struct queue { 
    struct customerNode *front, *rear; /* head, tail respectively */
};

/*----- function prototypes --------------------------------------------------*/

CN*    newNode  (unsigned int, unsigned int);

struct queue*   createQueue();
void   enQueue  (struct queue*, int);
void   deQueue  (struct queue*);
int    getCount (struct customerNode*);

#endif
