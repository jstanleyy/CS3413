// CS3413 - Operating Systems I
// Assignment 1 - Question 1
// Joel Stanley
// Due October 1, 2015

#include <stdio.h>
#include <stdlib.h>

struct Node {
	char *user;
	char process;
	int arrival;
	int duration;
	struct Node *next;
};

//Pointers for Queue
struct Node* front = NULL;
struct Node* end = NULL;

FILE *file;

void enqueue(char *userIn, char processIn, int arrivalIn, int durationIn) {
	struct Node* new = (struct Node*)malloc(sizeof(struct Node));
	new->user = userIn;
	new->process = processIn;
	new->arrival = arrivalIn;
	new->duration = durationIn;
	new->next = NULL;

	if(front == NULL && end == NULL) {
		front = end = new;
	}
	else {
		end->next = new;
		end = new;
	}
} 

int main() {
	
	file = fopen("data", "r");
	char line[1024];
	memset(line, 0, 1024);
	
	char tUser[10];
	char tProcess;
	int tArrival;
	int tDuration;
	
	fgets(line, 1024, file); // Skips the first line
	
	while(!feof(file)) {
		sscanf(line,"%s	%c	%d	%d", tUser, &tProcess, &tArrival, &tDuration);
		enqueue(tUser, tProcess, tArrival, tDuration);
		printf("Added");
	}
	fclose(file);
	int time = front->arrival;
	
	printf("\nTime	Job\n");
	
	while(front != NULL){
		printf("%d	%c\n", time, front->process);
		time += front->duration;
		front = front->next;
	}
	
	printf("%d	IDLE\n", time);
	return 0;
}


