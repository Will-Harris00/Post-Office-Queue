#ifndef __RANDOMDISTRIBUTIONS__
#define __RANDOMDISTRIBUTIONS__ 1

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/*----- function prototypes --------------------------------------------------*/

int chooseDistribution(int, int, unsigned int, gsl_rng **, unsigned int*);

#endif
