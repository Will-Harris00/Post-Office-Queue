#include <stdio.h>
#include <stdlib.h>

/*----- structure definition -------------------------------------------------*/

struct stackStruct
{
   char                  data;
   struct stackStruct   *next;
};
typedef struct stackStruct STACK;

/*----- function prototypes --------------------------------------------------*/

STACK *newNode    (char data);
void   push       (STACK **, char);
char   pop        (STACK **);
void   printStack (STACK *);
void   emptyStack (STACK **);

/*----- main function --------------------------------------------------------*/

int main ()
{
   STACK   *stack = NULL;

   push(&stack,'Q');
   push(&stack,'C');
   push(&stack,'A');
   pop(&stack);
   printStack(stack);

   emptyStack(&stack);
}
