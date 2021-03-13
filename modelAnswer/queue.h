#ifndef __QUEUE__
#define __QUEUE__

#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <parameters.h>
#include <customer.h>

struct nodeStruct
{
   CUSTOMER            *customer;
   struct nodeStruct   *prevNode;
   struct nodeStruct   *nextNode;
};
typedef struct nodeStruct NODE;

struct queueStruct
{
   int     maxLength;
   int     length;
   NODE   *head;
   NODE   *tail;
};
typedef struct queueStruct QUEUE;

extern QUEUE    *newQueue            (int);
extern void      freeQueue           (QUEUE **);
extern void      addQueueHead        (PARAMETERS *,QUEUE *,CUSTOMER *);
extern CUSTOMER *removeQueueTail     (PARAMETERS *,QUEUE *);
extern int       removeQueueAnywhere (PARAMETERS *,QUEUE *);
extern void      printQueue          (PARAMETERS *,QUEUE *,char *);

#endif
