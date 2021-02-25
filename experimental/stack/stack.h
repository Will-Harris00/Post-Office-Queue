#ifndef __STACK__
#define __STACK__ 1

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

void   push       (STACK **, char);
char   pop        (STACK **);
void   printStack (STACK *);
void   emptyStack (STACK **);

#endif
