#ifndef __QMAIN__
#define __QMAIN__

#define DEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue.h>
#include <parameters.h>

void        runSim                 (PARAMETERS *);
void        outputOneInterval      (PARAMETERS *,QUEUE *,QUEUE *);
PARAMETERS *getParameterValues     (int,char **);
void        customersFinishService (PARAMETERS *,QUEUE *);
void        customersStartService  (PARAMETERS *,QUEUE *,QUEUE *);
void        customersTimeout       (PARAMETERS *,QUEUE *);
void        customersArrive        (PARAMETERS *,QUEUE *);
void        runningStats           (float *,float *,float *,int,int);

#endif
