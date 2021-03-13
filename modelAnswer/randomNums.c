#include <randomNums.h>

/*****************************************************************************/
/* Set up and seed the random number generator.                              */
/*****************************************************************************/
extern gsl_rng *seedRandomNumbers ()
{
   /* create a random number generator */
   const gsl_rng_type *T;
   gsl_rng            *r;
   gsl_rng_env_setup();
   T = gsl_rng_default;
   r = gsl_rng_alloc(T);

   /* seed it */
   gsl_rng_set(r,time(0));

   return r;
}

/*****************************************************************************/
/* Free the memory associated with the random number generator.              */
/*****************************************************************************/
extern void freeRandomNumbers (gsl_rng **r)
{
   gsl_rng_free(*r);
}

/*****************************************************************************/
/* Generate a random number, from one of three available distributions:      */
/* DISTN_UNIFORM : [p1,p2] define the range of the distribution.             */
/* DISTN_GAUSSIAN: [p1,p2] define the mean and standard deviation.           */
/* DISTN_POISSON : p1 defines the mean; p2 is ignored.                       */
/*****************************************************************************/
extern float getRandomNumber (gsl_rng **r, char distribution, float p1, float p2)
{
   float   randomNum = 0.0;

   switch (distribution)
   {
      case DISTN_UNIFORM : randomNum = gsl_ran_flat(*r,p1,p2);       break;
      case DISTN_GAUSSIAN: randomNum = gsl_ran_gaussian(*r,p2)+p1;   break;
      case DISTN_POISSON : randomNum = gsl_ran_poisson(*r,p1);       break;
      default      : fprintf(stderr,"Invalid probability distribution %d specified\n",distribution);
                     exit(-6);
   }
   return randomNum;
}
