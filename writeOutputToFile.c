#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int writeOutputToFile(char*, char*, int);

int main(int argc, char **argv)
{
    char* fileOut = argv[1];
    char* description = argv[2];
    int data = atoi(argv[3]);

    writeOutputToFile(fileOut, description, data);
}

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
