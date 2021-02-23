#ifndef __WRITEOUTPUTTOFILE__
#define __WRITEOUTPUTTOFILE__ 1

#include <stdio.h>
#include <stdlib.h>

/*----- function prototypes --------------------------------------------------*/

int writeOutputToFile(char*, char*, char*);
int stopOverwrite(char *fileOut);
void writeParametersToFile(char*, int*, int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*);

#endif
