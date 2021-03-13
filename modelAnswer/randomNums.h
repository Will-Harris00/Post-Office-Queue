#ifndef __RANDOMNUMS__
#define __RANDOMNUMS__

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define DISTN_UNIFORM  'U'
#define DISTN_GAUSSIAN 'G'
#define DISTN_POISSON  'P'

extern gsl_rng *seedRandomNumbers ();
extern void     freeRandomNumbers (gsl_rng **);
extern float    getRandomNumber   (gsl_rng **, char, float, float);

#endif
