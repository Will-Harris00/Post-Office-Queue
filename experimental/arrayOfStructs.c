#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    typedef struct{
        char* firstName;
        char* lastName;
        int day;
        int month;
        int year;

    }STUDENT;

    int numStudents=3;
    int x;
    STUDENT* students = malloc(numStudents * sizeof *students);
    for (x = 0; x < numStudents; x++){
        students[x].firstName=(char*)malloc(sizeof(char*));
        scanf("%s",students[x].firstName);
        students[x].lastName=(char*)malloc(sizeof(char*));
        scanf("%s",students[x].lastName);
        scanf("%d",&students[x].day);
        scanf("%d",&students[x].month);
        scanf("%d",&students[x].year);
    }

    for (x = 0; x < numStudents; x++)
        printf("first name: %s, surname: %s, day: %d, month: %d, year: %d\n",students[x].firstName,students[x].lastName,students[x].day,students[x].month,students[x].year);

    return 0;
}
