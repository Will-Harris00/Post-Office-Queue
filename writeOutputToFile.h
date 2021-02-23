#ifndef __WRITEOUTPUTTOFILE__
#define __WRITEOUTPUTTOFILE__ 1

#include <stdio.h>
#include <stdlib.h>

/*----- function prototypes --------------------------------------------------*/

int writeOutputToFile(char*, char*, char *);
void unsignedTypeCasting(char*, char*, unsigned int*);
void signedTypeCasting(char*, char*, int*);
int stopOverwrite(char*);
void writeParametersToFile(char*, int*, int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*);

#endif
