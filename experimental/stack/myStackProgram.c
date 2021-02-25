#include <myStackProgram.h>

int main ()
{
   STACK   *stack = NULL;

   push(&stack,'Q');
   push(&stack,'C');
   push(&stack,'A');
   pop(&stack);

   printf("Current state of the stack:\n");
   printStack(stack);

   emptyStack(&stack);
}
