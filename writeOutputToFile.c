#include <writeOutputToFile.h>

/* functions to manage writing to output file */

int writeOutputToFile(char* fileOut, char *description, int data)
{
    FILE *fp;

    if ( (fp = fopen(fileOut, "a")) == NULL )
    {
        fprintf(stderr, "File not created\n");
        return -1;
    }
    fprintf(fp, "%s %d\n", description, data);

    fclose(fp);

    return 0;

}
