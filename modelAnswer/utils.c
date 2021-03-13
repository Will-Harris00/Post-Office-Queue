#include <utils.h>

extern void *allocateMemory (int numBytes, char *inFile, int inLine)
{
   void *p;
   if ( (p = malloc(numBytes)) == NULL )
   {
      fprintf(stderr,"Unable to allocate memory: %s, line %d\n",inFile,inLine);
      exit(-99);
   }
   return p;
}
