#include <stdio.h>
#include <stdlib.h>

/*----- structure definition  ----------------------- */
struct stackStruct{
    char data;
    struct stackStruct *nextNode;
};
typedef struct stackStruct STACK;

/* function prototypes ------------------------------ */
STACK *makeNewNode(char);
void push(STACK **, char); /* add node to stack */
char pop(STACK **); /* remove node from stack */
void printStack(STACK *); /* pass as a pointer so you are not copying the entire stack */
void emptyStack (STACK **);

/* main function ------------------------------------ */
int main(){
    STACK *stack = NULL; /* these nodes have no memory allocation to them */
    /* we want to update root so need to pass a pointer to root */
    /* STACK *newNode = makeNewNode('A'); this is now being done inside push */ 
    char data;

    push(&stack, 'Q'); /* because we want roots value to change */
    push(&stack, 'C'); /* even though root is iteself a pointer */
    push(&stack, 'A'); /* we still pass root as a pointer to push */
    pop(&stack);
    printStack(stack);

    emptyStack(&stack);
    return 0;
}

/* create a new node from the given data and return a pointer to it */
STACK *makeNewNode(char data){
    /* we must allocation memory to store data in the nodes */
    STACK *newNode = (STACK *)malloc(sizeof(STACK));
    if(newNode ==NULL){
        printf("Memory allocation error\n");
        exit(-1);
    }

    newNode->data = data; /* must assign memory before inserting data */
    newNode->nextNode = NULL; /* use arrors when we are assigning using pointers */

    return newNode;
}

/* push a value onto the top of the stack */
void push(STACK **stack,char data){
    STACK *newNode = makeNewNode(data); /* create new node */
    newNode->nextNode = *stack; /* new node points to existing node */
    *stack = newNode; /* stack points to new node */
}

/* pop a value off the top of the stack and return the value */
char pop (STACK **stack){
    STACK *node = *stack;
    char data = '\0';

    if (node==NULL){
        printf("Warning: attempting to pop from an empty stack.\n");
        printf("Warning: line %d, file %s\n", __LINE__, __FILE__);
        return data;
    }

    data = node->data;
    *stack = node->nextNode;
    free(node);
    return data;
}

/* recursively print out the contents of */
void printStack(STACK *stack){
    /* recursively print next node until NULL */
    if (stack !=NULL){
        printf("data = %c\n",stack->data);
        printStack(stack->nextNode);
    }
}

/* recursively empty the stack, thus freeing all the memory */
void emptyStack(STACK **stack){
    if (*stack==NULL)
        return;
    else {
        pop(stack);
        emptyStack(stack);
    }
}
