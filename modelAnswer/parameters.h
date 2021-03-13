#ifndef __PARAMETERS__
#define __PARAMETERS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <utils.h>
#include <randomNums.h>

struct statisticsStruct
{
   int     totalThisSim;
   int     totalAllSims;
   float   mean;
   float   standardDeviation;
   /* M is required for the online calculation of the standard deviation */
   float   M;
};
typedef struct statisticsStruct STATISTICS;


struct parametersStruct
{
   /* effectively, global variables */
   gsl_rng   *r;
   FILE      *fpout;
   int        numRepeats;
   int        runNumber;
   int        clock;
   /* parameters read in from the input file */
   int        numServicePoints;
   int        maxQueueLength;
   int        closingTime;
   char       customerArrivalsDistn;
   float      customerArrivalsP1;
   float      customerArrivalsP2;
   char       serviceTimesDistn;
   float      serviceTimesP1;
   float      serviceTimesP2;
   char       customerTimeoutDistn;
   float      customerTimeoutP1;
   float      customerTimeoutP2;
   /* calculated statistics */
   STATISTICS fulfilled;
   STATISTICS timedOut;
   STATISTICS unfulfilled;
   STATISTICS excessTime;
   STATISTICS customerWait;
};
typedef struct parametersStruct PARAMETERS;

extern PARAMETERS *readParameters       (FILE **,FILE *,int);
extern void        freeParameters       (PARAMETERS **);
extern void        resetSim             (PARAMETERS *);
extern void        printParameterValues (PARAMETERS *);
extern void        updateRunStatistics  (PARAMETERS *);
extern void        updateStatistics     (STATISTICS *, int);
extern void        outputStatistics     (PARAMETERS *);

#endif
