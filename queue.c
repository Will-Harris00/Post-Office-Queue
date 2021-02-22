#include <queue.h>

/* functions to manage the queue */

/* utility function to create a new linked list node. */
struct customerNode* newNode(unsigned int *custId, unsigned int patience)
{   
    struct customerNode* temp;
    if ( !( temp = (CN *)malloc(sizeof(CN)) ) ) /* check memory assignment */
    {
        fprintf(stderr, "Out of memory\n");
        exit(-1);
    }

    temp->customerId = *custId;
    temp->patience = patience;
    temp->timeElapsed = patience;
    temp->next = NULL;
    return temp;
}

/* utility function to create an empty queue */
struct queue* createQueue()
{
    struct queue* q;
    if ( !( q = (struct queue*)malloc(sizeof(struct queue)) ) ) /* check memory assignment */
    {
        fprintf(stderr, "Out of memory\n");
        exit(-1);
    }
    q->front = q->rear = NULL; 
    return q;
}

/* function to add a customerNode to the queue */
void enQueue(struct queue* q, int patience, unsigned int *custId)
{
    /* Create a new LL node */
    struct customerNode* temp = newNode(custId, patience); 
    ++(*custId);

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

/* function to remove a customerNode from the queue */
void deQueue(struct queue* q)
{
    /* If queue is empty, return NULL. */
    if ( q->front == NULL ) 
        return; 

    /* Store previous front and move front one node ahead */
    struct customerNode* temp = q->front;

    q->front = q->front->next;

    /* If front becomes NULL, then change rear also as NULL */
    if ( q->front == NULL )
        q->rear = NULL;

    free(temp);
}

/* function to count number of nodes in the queue */
int getCount(struct customerNode* head)
{ 
    /* Base case */
    if ( head == NULL )
        return 0;
  
    /* count is 1 + count of remaining list */
    return 1 + getCount(head->next);
}

/* function to remove customers from queue that have reached waiting tolerance limit */
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
