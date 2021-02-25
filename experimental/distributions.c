#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int main (){
    int i;
    float n;
    float nums[1000];
    const gsl_rng_type *T;
    gsl_rng            *r;
    /* create a random number generator */
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    /* seed it – equivalent of srand(time(0)) */
    gsl_rng_set(r,time(0));

    /* some code goes here */
    /* generate some Uniformly distributed random numbers */
    printf("Uniform Distribution: between 0 and 9\n");
    for (i=0; i<1000; i++){
        n = gsl_ran_flat(r,0,9);
	nums[i] = n;
    }
	
    for (i=0; i<1000; i++)
        printf("%f\n", nums[i]);

    gsl_rng_free(r);

    return 0;

}
