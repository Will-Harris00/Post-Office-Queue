#include <simQ.h>

/*****************************************************************************/
/* Usage: simQ <parameter-file-name> <number-of-simulations> <output-file>   */
/*****************************************************************************/
int main (int argc, char **argv)
{
   PARAMETERS *parms = getParameterValues(argc,argv);

   for (parms->runNumber=1; parms->runNumber<=parms->numRepeats; parms->runNumber++)
   {
      resetSim(parms);
      runSim(parms);
      updateRunStatistics(parms);
   }
   outputStatistics(parms);

   freeParameters(&parms);
   return EXIT_SUCCESS;
}

/*****************************************************************************/
/* Run a single simulation according to the parameter value in <parms>.      */
/*****************************************************************************/
void runSim (PARAMETERS *parms)
{
   QUEUE  *queue   = newQueue(parms->maxQueueLength);
   QUEUE  *service = newQueue(parms->numServicePoints);

   do
   {
      parms->clock++;
      customersFinishService(parms,service);
      customersStartService(parms,service,queue);
      customersTimeout(parms,queue);
      if ( parms->clock<=parms->closingTime )
         customersArrive(parms,queue);
      outputOneInterval(parms,service,queue);

      if ( DEBUG )
      {
         printQueue(parms,service,"Service points");
         printQueue(parms,queue,"Customer queue");
         printf("\n");
      }
   }
   while ( parms->clock<parms->closingTime || queue->length>0 || service->length>0 );

   freeQueue(&queue);
   freeQueue(&service);
}

/*****************************************************************************/
/* Output the details of each time interval in a simulation, but only if     */
/* is a single repeat run.                                                   */
/*****************************************************************************/
void outputOneInterval (PARAMETERS *parms,QUEUE *service,QUEUE *queue)
{
   if ( parms->numRepeats==1 )
   {
      fprintf(parms->fpout,"%04d: serving %4d, queue %4d, fulfilled %4d, unfulfilled %4d, timedOut %4d\n",
                           parms->clock, service->length, queue->length, 
                           parms->fulfilled.totalThisSim,
                           parms->unfulfilled.totalThisSim,
                           parms->timedOut.totalThisSim);

      if ( parms->clock==parms->closingTime )
         fprintf(parms->fpout,"---------- CLOSING TIME ----------\n");
   }
}

/*****************************************************************************/
/* Get the parameter values from the file, and set and seed the random       */
/* number generator.                                                         */
/*****************************************************************************/
PARAMETERS *getParameterValues (int argc,char **argv)
{
   PARAMETERS *parms;
   FILE       *fpin;
   FILE       *fpout;
   int         numRepeats = 0;

   if ( argc < 3 )
   {
      fprintf(stdout,"Usage: %s <fileIn> <numRepeats> [<fileOut>]\n", argv[0]);
      fprintf(stderr,"Usage: %s <fileIn> <numRepeats> [<fileOut>]\n", argv[0]);
      exit(-1);
   }
   numRepeats = atoi(argv[2]);
   if ( numRepeats == 0 )
   {
      fprintf(stdout,"Number of repeats must be an integer > 0\n");
      fprintf(stderr,"Number of repeats must be an integer > 0\n");
      exit(-2);
   }

   if ( (fpin = fopen(argv[1],"r")) == NULL )
   {
      fprintf(stderr,"Unable to open file %s for read access\n",argv[1]);
      exit(-3);
   }
   if ( argc < 4 )
      fpout = stdout;
   else if ( (fpout = fopen(argv[3],"w")) == NULL )
   {
      fprintf(stderr,"Unable to open file %s for write access\n",argv[3]);
      exit(-4);
   }

   parms = readParameters(&fpin,fpout,numRepeats);

   fclose(fpin);

   return parms;
}

/*****************************************************************************/
/* Remove (and count) customers whose service has been completed.            */
/*****************************************************************************/
void customersFinishService (PARAMETERS *parms, QUEUE *service)
{
   parms->fulfilled.totalThisSim += removeQueueAnywhere(parms,service);
}

/*****************************************************************************/
/* For any service points that do not have a customer being served, remove   */
/* the next customer from the queue and start serving them.                  */
/*****************************************************************************/
void customersStartService (PARAMETERS *parms, QUEUE *service, QUEUE *queue)
{
   CUSTOMER *c;

   while ( service->length<service->maxLength && queue->length>0 )
   {
      c = removeQueueTail(parms,queue);
      setCustomerService(parms,c);
      addQueueHead(parms,service,c);
      parms->customerWait.totalThisSim += parms->clock - c->arrivalTime;
   }
}

/*****************************************************************************/
/* Remove (and count) any customers from the queue that have exceeded their  */
/* waiting tolerance limit.                                                  */
/*****************************************************************************/
void customersTimeout (PARAMETERS *parms, QUEUE *queue)
{
   parms->timedOut.totalThisSim += removeQueueAnywhere(parms,queue);
}

/*****************************************************************************/
/* New customers arrive.  Either they join the queue, or, if the queue is    */
/* already full then count them as unfulfilled.                              */
/*****************************************************************************/
void customersArrive (PARAMETERS *parms, QUEUE *queue)
{
   int   numArrivals = (int)getRandomNumber(&parms->r, parms->customerArrivalsDistn, parms->customerArrivalsP1, parms->customerArrivalsP2);
   int   numUnfulfilled = 0;
   int   i;

   for ( i=1; i<=numArrivals; i++ )
   {
      if ( queue->length<queue->maxLength | queue->maxLength==-1 )
         addQueueHead(parms,queue,newCustomer(parms));
      else
         parms->unfulfilled.totalThisSim++;
   }
}

/*****************************************************************************/
/* calculates a running mean and standard deviation                          */
/* Uses Welford's algorithm; see                                             */
/* https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm */
/*****************************************************************************/
void runningStats (float *avg, float *std, float *M, int num, int n)
{
   float   prev_avg = *avg;
   float   prev_M   = *M;

   if ( n==1 )
   {
      *avg = num;
      *std = 0.0;
   }
   else
   {
      *avg = prev_avg + ((float)num-prev_avg)/n;
      *M   = prev_M + ((float)num-prev_avg)*((float)num-*avg);
      *std = pow(*M/n,0.5);
   }
}
