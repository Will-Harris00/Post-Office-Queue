#include <parameters.h>

#define BUFFERSIZE 81
#define TYPE_INTEGER 0
#define TYPE_CHAR    1
#define TYPE_FLOAT   2

static void readParameterValues  (PARAMETERS *,char *,char *);
static void getValue             (void *, char, char *);
static void initialiseStatistics (STATISTICS *);

/*****************************************************************************/
/* Set default values for all the parameters, and then read values in from   */
/* the parameter file.                                                       */
/*****************************************************************************/
extern PARAMETERS *readParameters (FILE **fpin, FILE *fpout, int numRepeats)
{
   PARAMETERS *parms;
   char        buffer[BUFFERSIZE];
   char        name[BUFFERSIZE];

   parms = (PARAMETERS *)allocateMemory(sizeof(PARAMETERS),__FILE__,__LINE__);

   /* initialise */
   resetSim(parms);
   parms->r = seedRandomNumbers();
   parms->fpout = fpout;
   parms->numRepeats = numRepeats;

   /* set default values */

   parms->numServicePoints = 3;
   parms->maxQueueLength   = 8;
   parms->closingTime      = 100;

   parms->customerArrivalsDistn = DISTN_POISSON;
   parms->customerArrivalsP1    = 3.0;
   parms->customerArrivalsP2    = 0.0;

   parms->serviceTimesDistn     = DISTN_GAUSSIAN;
   parms->serviceTimesP1        = 3.0;
   parms->serviceTimesP2        = 2.0;

   parms->customerTimeoutDistn  = DISTN_GAUSSIAN;
   parms->customerTimeoutP1     = 3.0;
   parms->customerTimeoutP2     = 2.0;

   initialiseStatistics(&parms->fulfilled);
   initialiseStatistics(&parms->timedOut);
   initialiseStatistics(&parms->unfulfilled);
   initialiseStatistics(&parms->excessTime);
   initialiseStatistics(&parms->customerWait);

   /* read in from the parameter file */
   while ( !feof(*fpin) )
   {
      fgets(buffer,BUFFERSIZE,*fpin);
      if ( strlen(buffer)>0   /* there is some text in the input line */ 
      &&   buffer[0]!='#' )   /* and it is not a comment */
      {
         if ( sscanf(buffer,"%s",name) == 1 )
            readParameterValues(parms,buffer+strlen(name),name);
      }
   }

   printParameterValues(parms);

   return parms;
}

/*****************************************************************************/
/* Free the parameters pointers.                                             */
/*****************************************************************************/
extern void freeParameters (PARAMETERS **parms)
{
   fclose((*parms)->fpout);
   freeRandomNumbers(&(*parms)->r);
   free(*parms);
   *parms = NULL;
}

/*****************************************************************************/
/* Reset parameter values for the start of a new simulation.                 */
/*****************************************************************************/
extern void resetSim (PARAMETERS *parms)
{
   parms->clock = 0;

   parms->fulfilled.totalThisSim    = 0;
   parms->unfulfilled.totalThisSim  = 0;
   parms->timedOut.totalThisSim     = 0;
   parms->excessTime.totalThisSim   = 0;
   parms->customerWait.totalThisSim = 0;
}

/*****************************************************************************/
/* Print out all the parameter values.                                       */
/*****************************************************************************/
extern void printParameterValues (PARAMETERS *parms)
{
   fprintf(parms->fpout,"Parameter values:\n");
   fprintf(parms->fpout,"-----------------\n");
   fprintf(parms->fpout,"numRepeats:            %d\n",parms->numRepeats);
   fprintf(parms->fpout,"numServicePoints:      %d\n",parms->numServicePoints);
   fprintf(parms->fpout,"maxQueueLength:        %d\n",parms->maxQueueLength);
   fprintf(parms->fpout,"closingTime:           %d\n",parms->closingTime);
   fprintf(parms->fpout,"customerArrivalsDistn: %c\n",parms->customerArrivalsDistn);
   fprintf(parms->fpout,"customerArrivalsP1:    %f\n",parms->customerArrivalsP1);
   fprintf(parms->fpout,"customerArrivalsP2:    %f\n",parms->customerArrivalsP2);
   fprintf(parms->fpout,"serviceTimesDistn:     %c\n",parms->serviceTimesDistn);
   fprintf(parms->fpout,"serviceTimesP1:        %f\n",parms->serviceTimesP1);
   fprintf(parms->fpout,"serviceTimesP2:        %f\n",parms->serviceTimesP2);
   fprintf(parms->fpout,"customerTimeoutDistn:  %c\n",parms->customerTimeoutDistn);
   fprintf(parms->fpout,"customerTimeoutP1:     %f\n",parms->customerTimeoutP1);
   fprintf(parms->fpout,"customerTimeoutP2:     %f\n",parms->customerTimeoutP2);
   fprintf(parms->fpout,"\n");
}

/*****************************************************************************/
/* Given a string and the name of the parameter, extract the numeric value   */
/* of that parameter from the string.  Different parameters have different   */
/* types of values: char, int or float.                                      */
/*****************************************************************************/
static void readParameterValues (PARAMETERS *parms, char *buffer, char *name)
{
   while ( buffer[0]==' ' )
      buffer++;
   if ( !strcmp(name,"numServicePoints") )
      getValue(&parms->numServicePoints,TYPE_INTEGER,buffer);
   else if ( !strcmp(name,"maxQueueLength") )
      getValue(&parms->maxQueueLength,TYPE_INTEGER,buffer);
   else if ( !strcmp(name,"closingTime") )
      getValue(&parms->closingTime,TYPE_INTEGER,buffer);
   else if ( !strcmp(name,"customerArrivalsDistn") )
      getValue(&parms->customerArrivalsDistn,TYPE_CHAR,buffer);
   else if ( !strcmp(name,"customerArrivalsP1") )
      getValue(&parms->customerArrivalsP1,TYPE_FLOAT,buffer);
   else if ( !strcmp(name,"customerArrivalsP2") )
      getValue(&parms->customerArrivalsP2,TYPE_FLOAT,buffer);
   else if ( !strcmp(name,"serviceTimesDistn") )
      getValue(&parms->serviceTimesDistn,TYPE_CHAR,buffer);
   else if ( !strcmp(name,"serviceTimesP1") )
      getValue(&parms->serviceTimesP1,TYPE_FLOAT,buffer);
   else if ( !strcmp(name,"serviceTimesP2") )
      getValue(&parms->serviceTimesP2,TYPE_FLOAT,buffer);
   else if ( !strcmp(name,"customerTimeoutDistn") )
      getValue(&parms->customerTimeoutDistn,TYPE_CHAR,buffer);
   else if ( !strcmp(name,"customerTimeoutP1") )
      getValue(&parms->customerTimeoutP1,TYPE_FLOAT,buffer);
   else if ( !strcmp(name,"customerTimeoutP2") )
      getValue(&parms->customerTimeoutP2,TYPE_FLOAT,buffer);
   else
      fprintf(stderr,"Warning: unexpected parameter name: %s\n",name);
}

/*****************************************************************************/
/* Extract a parameter value of a given type from the buffer.                */
/*****************************************************************************/
static void getValue (void *variable, char type, char *buffer)
{
   char    numScanned;
   char    cvalue = ' ';
   int     ivalue = 0;
   float   fvalue = 0.0;

   switch (type)
   {
      case TYPE_INTEGER: numScanned = sscanf(buffer,"%d",&ivalue);   break;
      case TYPE_CHAR   : numScanned = sscanf(buffer,"%c",&cvalue);   break;
      case TYPE_FLOAT  : numScanned = sscanf(buffer,"%f",&fvalue);   break;
      default          : numScanned = 0;
   }

   if ( numScanned != 1 )
      fprintf(stderr,"Warning: unable to extract parameter value: %s",buffer);
   else if ( type==TYPE_CHAR )
      *(char *)variable = cvalue;
   else if ( type==TYPE_INTEGER )
      *(int *)variable = ivalue;
   else
      *(float *)variable = fvalue;
}

/*****************************************************************************/
/* Initialise the statistics to all zero values before a simulation run.     */
/*****************************************************************************/
static void initialiseStatistics (STATISTICS *s)
{
   s->totalThisSim      = 0;
   s->totalAllSims      = 0;
   s->mean              = 0.0;
   s->standardDeviation = 0.0;
   s->M                 = 0.0;
}

/*****************************************************************************/
/* Updates the statistics at the end of a single simulation run.             */
/*****************************************************************************/
extern void updateRunStatistics (PARAMETERS *parms)
{
   parms->excessTime.totalThisSim = parms->clock - parms->closingTime;

   updateStatistics(&parms->fulfilled,    parms->runNumber);
   updateStatistics(&parms->unfulfilled,  parms->runNumber);
   updateStatistics(&parms->timedOut,     parms->runNumber);
   updateStatistics(&parms->excessTime,   parms->runNumber);
   updateStatistics(&parms->customerWait, parms->fulfilled.totalThisSim * parms->runNumber);
}

/*****************************************************************************/
/* Firstly increments the running total, and then updates the current        */
/* estimate of the mean and standard deviation, using Welford's algorithm;   */
/* see:                                                                      */
/* https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm */
/*****************************************************************************/
extern void updateStatistics (STATISTICS *s, int n)
{
   float   prev_mean = s->mean;
   float   prev_M    = s->M;

   s->totalAllSims += s->totalThisSim;

   /* n is how many things we have counted so far */
   /* i.e. the simulation run number              */
   if ( n==1 )
   {
      s->mean              = s->totalThisSim;
      s->standardDeviation = 0.0;
   }
   else
   {
      s->mean              = prev_mean + ((float)s->totalThisSim - prev_mean)/n;
      s->M                 = prev_M    + ((float)s->totalThisSim - prev_mean) * ((float)s->totalThisSim - s->mean);
      s->standardDeviation = pow(s->M/n,0.5);
   }
}

/*****************************************************************************/
/* At the end of an execution, output the customer service statistics.       */
/*****************************************************************************/
extern void outputStatistics (PARAMETERS *parms)
{
   if ( parms->numRepeats==1 )
   {
      fprintf(parms->fpout,"\nSummary for one simulation:\n");
      fprintf(parms->fpout,"   excess time   : %d\n",parms->clock - parms->closingTime);
      fprintf(parms->fpout,"   numFulfilled  : %d\n",parms->fulfilled.totalThisSim);
      fprintf(parms->fpout,"   numUnfulfilled: %d\n",parms->unfulfilled.totalThisSim);
      fprintf(parms->fpout,"   numTimedOut   : %d\n",parms->timedOut.totalThisSim);
      fprintf(parms->fpout,"   avg wait      : %f\n",(float)parms->customerWait.totalThisSim/parms->fulfilled.totalThisSim);
   }
   else
   {
      fprintf(parms->fpout,"\nSummary for multiple simulations:\n");
      fprintf(parms->fpout,"   ExcessTime : mean %f, std %f\n",parms->excessTime.mean,parms->excessTime.standardDeviation);
      fprintf(parms->fpout,"   Fulfilled  : mean %f, std %f\n",parms->fulfilled.mean,parms->fulfilled.standardDeviation);
      fprintf(parms->fpout,"   Unfulfilled: mean %f, std %f\n",parms->unfulfilled.mean,parms->unfulfilled.standardDeviation);
      fprintf(parms->fpout,"   TimedOut   : mean %f, std %f\n",parms->timedOut.mean,parms->timedOut.standardDeviation);
      fprintf(parms->fpout,"   Avg Wait   : mean %f, std %f\n",parms->customerWait.mean,parms->customerWait.standardDeviation);
   }
}
