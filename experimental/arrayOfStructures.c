/* C program to demonstrate the */
/* usage of an array of structures */
#include <stdio.h> 

/* Declaring a structure class */
struct class { 
	int roll_no; 
	char grade; 
	float marks; 
}; 

/* Function to displays the contents */
/* of the array of structures */
void display(struct class class_record[3]) 
{ 
	int i, len = 3; 

	/* Display the contents of the array */
	/* of structures here, each element */
	/* of the array is a structure of class */
	for (i = 0; i < len; i++) { 
		printf("Roll number : %d\n", 
			class_record[i].roll_no); 
		printf("Grade : %c\n", 
			class_record[i].grade); 
		printf("Average marks : %.2f\n", 
			class_record[i].marks); 
		printf("\n"); 
	} 
} 

/* Driver Code */
int main() 
{ 
	/* Initialize of an array of structures */
	struct class class_record[3] 
		= { { 1, 'A', 89.5f }, 
			{ 2, 'C', 67.5f }, 
			{ 3, 'B', 70.5f } }; 

	/* Function Call to display */
	/* the class_record */
	display(class_record); 
	return 0; 
} 
