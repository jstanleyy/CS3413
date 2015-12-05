// CS3413 - Operating Systems I
// Assignment 5 - Question 1
// Joel Stanley
// Due December 7, 2015

#include <stdio.h>
#include <stdlib.h>

// Global Variables.
FILE *file;

// Function Declarations
int FCFS(int head);
/*int SSTF(int head);
int SCAN(int head);
int CSCAN(int head);
int LOOK(int head);
int CLOOK(int head); */

int main (int argc, char *argv[]) {
	
	// Checks for correct arguments.
	if(argc != 3) {
		printf("Missing Arguments");
		return(1);
	}
	
	file = fopen("cylinder_requests.txt", "r"); // Opens the requests for reading.
	if(file == 0) {
		printf("Could not open file.");
	}
	
	int head = atoi(argv[2]);
	int movement = 0;
	//printf("The head starts at %d\n", head);
	//printf("Selection: %c\n", *argv[1]);
	
	switch(*argv[1]) {
		case 'F' :
			movement = FCFS(head);
			break;
		case 'T' :
			//movement = SSTF(head);
			break;
		case 'S' :
			//movement = SCAN(head);
			break;
		case 'C' :
			//movement = CSCAN(head);
			break;
		case 'L' :
			//movement = LOOK(head);
			break;
		case 'O' :
			//movement = CLOOK(head);
			break;
		default :
			printf("Selection is invalid");
			return(1);
	}
	
	printf("The head movement required was: %d\n", movement);
}

int FCFS(int head) {
	
	//printf("In FCFC!\n");
	int movement = 0, current = 0;
	fscanf(file, "%d", current);
	printf("%d\n", current);
	return abs(head - current);
}