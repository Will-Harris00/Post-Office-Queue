#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int main (){
    unsigned int i;
    unsigned int existsGSL = 0;
    int n;
    int nums[100];
    gsl_rng            *r;
    if (!existsGSL)
    {
        const gsl_rng_type *T;
        /* create a random number generator */
        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);
        existsGSL = 1;
    }

    /* seed it based on the current time */
    gsl_rng_set(r,time(0));

    /* Uniform Distribution: between 0 and 9 */
    /* n = gsl_ran_flat(r,0,9); */
    /* Normal/Gaussian Distribution: mean 5, standard deviation 2 */
    /* n = gsl_ran_gaussian(r,2)+5; */
    /* Poisson Distribution: mean 5, standard deviation 2 */
    /* n = gsl_ran_poisson(r,2)+5; */
    /* Gamma Distribution: between 0 and 9 */
    /* n = gsl_ran_gamma(r,0,9); */

    printf("Uniform Distribution: between 0 and 9\n");
    for (i=0; i<20; i++){
        n = gsl_ran_flat(r,0,9);
	    nums[i] = n;
    }

    for (i=0; i<20; i++)
        printf("%d\n", nums[i]);


    printf("Normal/Gaussian Distribution: mean 4, standard deviation 0\n");
    for (i=0; i<100; i++){
        n = gsl_ran_gaussian(r,4)+0;
        while ( n < 0 )
            n = gsl_ran_gaussian(r,4)+0;
	    nums[i] = n;
    }
	
    for (i=0; i<100; i++)
        printf("%d\n", nums[i]);


    printf("Poisson Distribution: mean 0, standard deviation 4\n");
    for (i=0; i<100; i++){
        n = gsl_ran_poisson(r,4)+0;
	    nums[i] = n;
    }

    for (i=0; i<100; i++)
        printf("%d\n", nums[i]);


    printf("Gamma Distribution: between 0 and ∞\n");
    for (i=0; i<20; i++){
        n = gsl_ran_gamma(r,1,20);
	    nums[i] = n;
    }

    for (i=0; i<20; i++)
        printf("%d\n", nums[i]);

    gsl_rng_free(r);

    return 0;

}
