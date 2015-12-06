// CS3413 - Operating Systems I
// Assignment 5 - Question 1
// Joel Stanley
// Due December 7, 2015

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node {
	int data;
	struct Node *next;
	struct Node *prev;
};

// Global Variables.
FILE *file;
int count = 0;

//Pointers for Queue.
struct Node* front = NULL;
struct Node* end = NULL;

// Function Declarations
int FCFS(int head);
int SSTF(int head);
int SCAN(int head);
int CSCAN(int head);
int LOOK(int head);
int CLOOK(int head);
void enqueue(int dataIn);
void dequeue(int dataIn);
void printList();

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

	switch(*argv[1]) {
		case 'F' :
			movement = FCFS(head);
			break;
		case 'T' :
			movement = SSTF(head);
			break;
		case 'S' :
			movement = SCAN(head);
			break;
		case 'C' :
			movement = CSCAN(head);
			break;
		case 'L' :
			movement = LOOK(head);
			break;
		case 'O' :
			movement = CLOOK(head);
			break;
		default :
			printf("Selection is invalid");
			return(1);
	}
	
	printf("The head movement required was: %d\n", movement);
}

int FCFS(int head) {
	
	int seek = 0, current = 0, done = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			seek += abs(head - current);
			head = current;
		}
		else {
			done = 1;
		}
	}
	return seek;
}

int SSTF(int head) {
	
	// Reads in the requests and stores them in a sorted list.
	int seek = 0, done = 0, current = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			enqueue(current);
			count++;
		}
		else {
			done = 1;
		}
	}
	printf("HERE");
	while(front != NULL && end != NULL) {
		// Finds the request with the smallest seek time.
		int smallestDif = INT_MAX, smallestDifData = 0, i, dif;
		struct Node* check = front;
		for(i = 0; i < count; i++) {
			dif = abs(head - check->data);
			if(dif < smallestDif) {
				smallestDif = dif;
				smallestDifData = check->data;
			}
			check = check->next;
		}
		
		seek += smallestDif;
		
		dequeue(smallestDifData);
		count--;
		head = smallestDifData;
	}
	
	return seek;
}

int SCAN(int head) {
	
	// Reads in the requests and stores them in a sorted list.
	int seek = 0, done = 0, current = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			enqueue(current);
			count++;
		}
		else {
			done = 1;
		}
	}

	int direction = 0;
	while(front != NULL && end != NULL) {
		int i, closest;
		
		struct Node* check = front;
		for(i = 0; i < count; i++) {
			if(head < check->data) {
				if(direction == 0) {
					closest = check->prev->data;
					break;
				}
				else {
					closest = check->data;
					break;
				}
			}
		
			check = check->next;
		}
		
		seek += abs(head - closest);
		dequeue(closest);
		count--;
		head = closest;
		
		if(check == front) {
			if(direction == 0) {
				direction = 1;
				seek += head;
				head = 0;
			}			
		}
	}
	return seek;
}

int CSCAN(int head) {
	
	// Reads in the requests and stores them in a sorted list.
	int seek = 0, done = 0, current = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			enqueue(current);
			count++;
		}
		else {
			done = 1;
		}
	}
	
	int flag = 0;
	while(front != NULL && end != NULL) {
		int i, closest;
		
		struct Node* check = front;
		for(i = 0; i < count; i++) {
			if(head < check->data) {
				closest = check->data;
				break;
			}
			
			check = check->next;
		}
	
		seek += abs(head - closest);
		head = closest;
		
		if(check == end && flag == 0) {
			seek += (9999 - head);
			head = 0;
			flag = 1;
		}
		
		dequeue(closest);
		count--;
	}
	
	return seek;
}

int LOOK(int head) {
	// Reads in the requests and stores them in a sorted list.
	int seek = 0, done = 0, current = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			enqueue(current);
			count++;
		}
		else {
			done = 1;
		}
	}

	int direction = 0;
	while(front != NULL && end != NULL) {
		int i, closest;
		
		struct Node* check = front;
		for(i = 0; i < count; i++) {
			if(head < check->data) {
				if(direction == 0) {
					closest = check->prev->data;
					break;
				}
				else {
					closest = check->data;
					break;
				}
			}
		
			check = check->next;
		}
		
		seek += abs(head - closest);
		dequeue(closest);
		count--;
		head = closest;
		
		if(check == front) {
			if(direction == 0) {
				direction = 1;
			}			
		}
	}
	return seek;
}

int CLOOK(int head) {
	
	// Reads in the requests and stores them in a sorted list.
	int seek = 0, done = 0, current = 0;
	while(done == 0) {
		if(fscanf(file, "%d", &current) == 1) {
			enqueue(current);
			count++;
		}
		else {
			done = 1;
		}
	}
	
	int flag = 0;
	while(front != NULL && end != NULL) {
		int i, closest;
		
		struct Node* check = front;
		for(i = 0; i < count; i++) {
			if(head <= check->data) {
				closest = check->data;
				break;
			}
			
			check = check->next;
		}
	
		seek += abs(head - closest);
		head = closest;
		
		if(check == end && flag == 0) {
			seek +=  (9999 - (end->data - front->data));
			head = front->data;
			flag = 1;
		}
		dequeue(closest);
		count--;
	}
	
	return seek;
}

void enqueue(int dataIn) {
	
	struct Node* new = (struct Node*)malloc(sizeof(struct Node));
	new->data = dataIn;
	new->next = NULL;
	new->prev = NULL;
	
	if(front == NULL && end == NULL) {
		front = end = new;
	}
	else {
		struct Node* check = front;
		int i;
		for(i = 0; i <= count + 1; i++) {
			if(new->data == check->data) {
				// Adds to the end of the list.
				if(check == end) {
					check->next = new;
					new->prev = check;
					end = new;
					break;
				}
				else {
					struct Node* temp = check->next;
					check->next = new;
					new->next = temp;
					new->prev = check;
					temp->prev = new;
					break;
				}
			}
			else if(new->data < check->data) {
				// Adds to the front of the list.
				if(check == front) {
					new->next = front;
					front->prev = new;
					front = new;
					break;
				}
				else {
					struct Node* temp = check->prev;
					temp->next = new;
					new->next = check;
					new->prev = check->prev;
					check->prev = new;
					break;
				}
			}
			else if(check->next == NULL) {
				// Adds to the end of the list.
				check->next = new;
				new->prev = check;
				end = new;
				break;
			}
			else {
				check = check->next;
			}
		}
	}
}

void dequeue(int dataIn) {
	
	int i;
	struct Node* check = front;
	
	for(i = 0; i < count; i++) {
		if(check->data == dataIn) {
			if(front == end) {
				front = end = NULL;
			}
			else if(check == front) {
				front = front->next;
				front->prev = NULL;
				check->next = NULL;
				break;
			}
			else if(check == end) {
				end = end->prev;
				end->next = NULL;
				check->prev = NULL;
				break;
			}
			else {
				check->prev->next = check->next;
				check->next->prev = check->prev;
			}
		}
		
		check = check->next;
	}
	
}

// Used for debugging.
void printList() {
	int i;
	struct Node* check = front;
	for(i = 0; i < count; i++) {			
		printf("%d\n", check->data);
		check = check->next;
	}
	printf("\n");
}
