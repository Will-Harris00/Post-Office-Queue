#ifndef __CUSTOMER__
#define __CUSTOMER__

#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <parameters.h>

struct customerStruct
{
   int   customerNum;
   int   arrivalTime;
   int   maxWait;
};
typedef struct customerStruct CUSTOMER;

extern CUSTOMER *newCustomer        (PARAMETERS *);
extern CUSTOMER *copyCustomer       (CUSTOMER *);
extern void      setCustomerWait    (PARAMETERS *,CUSTOMER *);
extern void      setCustomerService (PARAMETERS *,CUSTOMER *);

#endif
