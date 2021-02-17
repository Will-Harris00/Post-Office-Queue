#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *fp;
    char attribute[51];
    int data;

    /* short int is not big enough so would overwrite
       the start of name and cause to terminate early */

    fp = fopen("input.txt", "r");
    if ( fp==NULL )
    {
        fprintf(stderr, "File not openable\n");
        return -1;
    }

    while ( !feof(fp) )
    {
        if ( fscanf(fp, "%s %d\n",
                                   attribute,
                                   &data) != 2 )
        {
            fprintf(stderr,"File format invalid\n");
            fclose(fp);
            return -2;
        }
        printf("attribute: %s, data: %u\n", attribute, data);
    }

    fclose(fp);

    return 0;
}
