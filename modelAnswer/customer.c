#include <customer.h>

/*****************************************************************************/
/* Create a new customer.                                                    */
/*****************************************************************************/
extern CUSTOMER *newCustomer (PARAMETERS *parms)
{
   static int   customerCount = 0;
   CUSTOMER    *c = (CUSTOMER *)allocateMemory(sizeof(CUSTOMER),__FILE__,__LINE__);

   customerCount++;
   c->customerNum = customerCount;
   c->arrivalTime = parms->clock;
   setCustomerWait(parms,c);

   return c;
}

/*****************************************************************************/
/* Copy a customer record from a pointer.                                    */
/*****************************************************************************/
extern CUSTOMER *copyCustomer (CUSTOMER *cold)
{
   CUSTOMER *c = (CUSTOMER *)allocateMemory(sizeof(CUSTOMER),__FILE__,__LINE__);

   c->customerNum = cold->customerNum;
   c->arrivalTime = cold->arrivalTime;
   c->maxWait     = cold->maxWait;

   return c;
}

/*****************************************************************************/
/* Set the waiting tolerance limit for this customer.                        */
/*****************************************************************************/
extern void setCustomerWait (PARAMETERS *parms, CUSTOMER *c)
{
   c->maxWait = (int)getRandomNumber(&parms->r, parms->customerTimeoutDistn, parms->customerTimeoutP1, parms->customerTimeoutP2);
   if ( c->maxWait<1 )
      c->maxWait = 1;
}

/*****************************************************************************/
/* Set the time it will take to serve this customer.                         */
/*****************************************************************************/
extern void setCustomerService (PARAMETERS *parms, CUSTOMER *c)
{
   c->maxWait = (int)getRandomNumber(&parms->r, parms->serviceTimesDistn, parms->serviceTimesP1, parms->serviceTimesP2);
   if ( c->maxWait<1 )
      c->maxWait = 1;
}
