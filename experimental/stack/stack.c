#include <stack.h>

/*----- local function prototypes --------------------------------------------*/

static STACK *newNode    (char data);

/*----- stack functions ------------------------------------------------------*/

/* create a new node from the given data and return a pointer to it */
static STACK *newNode (char data)
{
   STACK *s = (STACK *)malloc(sizeof(STACK));
   if ( s==NULL )
   {
      printf("Memory allocation error\n");
      exit(-1);
   }
   s->data = data;
   s->next = NULL;
   return s;
}

/* push a value onto the top of the stack */
extern void push (STACK **stack, char data)
{
   STACK   *s = newNode(data);
   s->next = *stack;
   *stack = s;
}

/* pop a value off the top of the stack and return the value */
extern char pop (STACK **stack)
{
   STACK   *s    = *stack;
   char     data = '\0';

   if ( s==NULL )
   {
      printf("Warning: attempting to pop from an empty stack.\n");
      printf("Warning: line %d, file %s\n",__LINE__,__FILE__);
      return data;
   }

   data = s->data;
   *stack = s->next;
   free(s);
   return data;
}

/* recursively print out the contents of the stack */
extern void printStack (STACK *stack)
{
   if ( stack==NULL )
   {
      printf("\n");
      return;
   }
   else
   {
      printf("%c",stack->data);
      printStack(stack->next);
   }
}

/* recursively empty the stack, thus freeing all the memory */
extern void emptyStack (STACK **stack)
{
   if ( *stack==NULL )
      return;
   else
   {
      pop(stack);
      emptyStack(stack);
   }
}
