#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int main (){
    int i;
    int n;
    int nums[1000];
    const gsl_rng_type *T;
    gsl_rng            *r;
    /* create a random number generator */
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    /* seed it based on the current time */
    gsl_rng_set(r,time(0));

    /* Uniform Distribution: between 0 and 9 */
    n = (int)gsl_ran_flat(r,0,9);
    /* Normal/Gaussian Distribution: mean 5, standard deviation 2 */
    n = (int)gsl_ran_gaussian(r,2)+5;
    /* Poisson Distribution: mean 5, standard deviation 2*/
    n = gsl_ran_poisson(r,2)+5;
    /* Gamma Distribution: */
    n = (int)gsl_ran_gamma(r,0,9);

    /* some code goes here */
    /* generate some Uniformly distributed random numbers */
    printf("Uniform distributed between 0 and 9\n");
    for (i=0; i<1000; i++){
        n = gsl_ran_flat(r,0,9);
	nums[i] = n;
    }
	
    for (i=0; i<1000; i++)
        printf("%f\n", nums[i]);

    free(r);

    return 0;

}
