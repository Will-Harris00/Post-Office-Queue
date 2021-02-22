#include <randomDistributions.h>

int chooseDistribution(int mean, int standardDeviation, unsigned int distribution, gsl_rng **r, unsigned int *existsGSL)
{
    int n;
    if ( !(*existsGSL) )
    {
        const gsl_rng_type *T;
        /* create a random number generator */
        gsl_rng_env_setup();
        T = gsl_rng_default;
        *r = gsl_rng_alloc(T);
        /* seed it based on the current time */
        gsl_rng_set(*r,time(0));
        (*existsGSL) = 1; /* ensures we only create the random number generator once */
    }

    if ( distribution == 1 )
    {
        printf("Uniform distributed between 0 and %d\n", mean);
        n = gsl_ran_flat(*r,0,mean);
    }

    if ( distribution == 2 )
    {
        printf("Normal/Gaussian Distribution: mean %d, standard deviation %d\n", mean, standardDeviation);
        n = gsl_ran_gaussian(*r,standardDeviation)+mean;
        while ( n < 0 )
            n = gsl_ran_gaussian(*r,standardDeviation)+mean;
    }

    if ( distribution == 3 )
    {
        printf("Poisson Distribution: mean %d, standard deviation %d\n", mean, standardDeviation);
        n = gsl_ran_poisson(*r,standardDeviation)+mean;
    }

    if ( distribution == 4 )
    {
        printf("Gamma Distribution: between 0 and ∞\n");
        n = gsl_ran_gamma(*r,1,mean);
    }

    return n;

}
