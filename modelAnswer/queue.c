#include <queue.h>

static NODE *newNode  (CUSTOMER *);
static void  freeNode (NODE **);

/*****************************************************************************/
/* Create a new, empty queue.                                                */
/*****************************************************************************/
extern QUEUE *newQueue (int maxLength)
{
   QUEUE *q = (QUEUE *)allocateMemory(sizeof(QUEUE),__FILE__,__LINE__);

   q->maxLength = maxLength;
   q->length    = 0;
   q->head      = NULL;
   q->tail      = NULL;

   return q;
}

/*****************************************************************************/
/* Free the memory associated with an empty queue.                           */
/*****************************************************************************/
extern void freeQueue (QUEUE **q)
{
   free(*q);
   *q = NULL;
}

/*****************************************************************************/
/* Create a new node for insertion into a queue.                             */
/*****************************************************************************/
static NODE *newNode (CUSTOMER *c)
{
   NODE *n = (NODE *)allocateMemory(sizeof(NODE),__FILE__,__LINE__);

   n->customer = c;
   n->prevNode = NULL;
   n->nextNode = NULL;

   return n;
}

/*****************************************************************************/
/* Free the memory associated with one node in the queue.                    */
/*****************************************************************************/
static void freeNode (NODE **n)
{
   free((*n)->customer);
   free(*n);
   *n = NULL;
}

/*****************************************************************************/
/* Add a new customer to the start of the queue.                             */
/*****************************************************************************/
extern void addQueueHead (PARAMETERS *parms, QUEUE *q, CUSTOMER *c)
{
   NODE *n = newNode(c);
   if ( q->head == NULL )   /* add first node */
   {
      q->head = n;
      q->tail = n;
   }
   else                     /* at least one node in the queue */
   {
      q->head->prevNode = n;
      n->nextNode = q->head;
      q->head     = n;
   }
   q->length++;
}

/*****************************************************************************/
/* Remove the last customer in the queue.  This is the next customer to be   */
/* served.                                                                   */
/*****************************************************************************/
extern CUSTOMER *removeQueueTail (PARAMETERS *parms, QUEUE *q)
{
   NODE     *n = q->tail;
   CUSTOMER *c = copyCustomer(n->customer);

   if ( q->head != q->tail )
   {
      q->tail = q->tail->prevNode;
      q->tail->nextNode = NULL;
   }
   else
   {
      q->head = NULL;
      q->tail = NULL;
   }

   freeNode(&n);
   q->length--;
   return c;
}

/*****************************************************************************/
/* For each customer in the queue, decrement their maxWait value.  If that   */
/* is now zero, then remove them from the queue.                             */
/*****************************************************************************/
extern int removeQueueAnywhere (PARAMETERS *parms, QUEUE *q)
{
   NODE *n = q->head;
   NODE *temp;
   int   numRemoved = 0;

   while ( n != NULL )
   {
      n->customer->maxWait--;
      if ( n->customer->maxWait == 0 )
      {
         if ( q->head == q->tail )         /* only node in the queue */
         {
            freeNode(&n);
            n       = NULL;
            q->head = NULL;
            q->tail = NULL;
         }
         else if ( n == q->head )          /* first node in the queue */
         {
            q->head = n->nextNode;
            q->head->prevNode = NULL;
            freeNode(&n);
            n = q->head;
         }
         else if ( n == q->tail )          /* last node in the queue */
         {
            q->tail = n->prevNode;
            q->tail->nextNode = NULL;
            freeNode(&n);
            n = NULL;
         }
         else                              /* middle node */
         {
            n->prevNode->nextNode = n->nextNode;
            n->nextNode->prevNode = n->prevNode;
            temp = n->nextNode;
            freeNode(&n);
            n = temp;
         }
         numRemoved++;
         q->length--;
      }
      else
         n = n->nextNode;
   }

   return numRemoved;
}

/*****************************************************************************/
/* Print out the customers in the queue.                                     */
/*****************************************************************************/
extern void printQueue (PARAMETERS *parms, QUEUE *q, char *title)
{
   int   i = 0;
   NODE *n = q->head;

   printf("----- %s ---------------------------------- %d\n",title,parms->clock);
   while ( n!=NULL )
   {
      i++;
      printf("   %3d: customerNum %03d, arrivalTime %d, maxWait %d\n",
             i,n->customer->customerNum,n->customer->arrivalTime,n->customer->maxWait);
      n = n->nextNode;
   }
   printf("   999: queue length %d (max %d)\n",q->length,q->maxLength);
}
