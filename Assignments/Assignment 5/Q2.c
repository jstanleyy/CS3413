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

struct diskNode {
	int block;
	int nextBlock;
	struct diskNode *next;
};

// Global Variables.
FILE *file;
FILE *disk;
int count = 0;

//Pointers for Queue.
struct Node* front = NULL;
struct Node* end = NULL;

struct diskNode* diskFront = NULL;
struct diskNode* diskEnd = NULL;

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
void initDisk();
void diskAdd(int blockIn, int nextIn);

int main (int argc, char *argv[]) {
	
	// Checks for correct arguments.
	if(argc != 4) {
		printf("Missing Arguments");
		return(1);
	}
	
	file = fopen("files_requested.txt", "r"); // Opens the requests for reading.
	if(file == 0) {
		printf("Could not open file.");
		return 1;
	}

	disk = fopen(argv[3], "r");
	if(disk == 0) {
		printf("Could not open disk.");
		return 1;
	}
	
	int head = atoi(argv[2]);
	int movement = 0;
	
	initDisk();
	
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
	
	int seek = 0, done = 0;
	while(done == 0) {
		int current = 0, nextBlock = 0;
		if(fscanf(file, "%d", &current) == 1) {
			printf("Path for file %d: ", current);
			while(nextBlock != -1) {
				struct diskNode* curNode = diskFront;
				while(curNode->block != current) {
					curNode = curNode->next;
				}
				
				seek += abs(head - curNode->block);
				nextBlock = curNode->nextBlock;
				current = nextBlock;
				head = curNode->block;
				printf("%d -> ", curNode->block);
			}
			printf("DONE\n");
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
		
		//printf("DIF: %d, DATA: %d\n", smallestDif, smallestDifData);
		seek += smallestDif;
		head = smallestDifData;
		printf("Path for file %d: ", smallestDifData);
		int nextBlock = smallestDifData;
		while(nextBlock != -1) {
			struct diskNode* curNode = diskFront;
			while(curNode->block != nextBlock) {
				curNode = curNode->next;
			}
				
			seek += abs(head - curNode->block);
			nextBlock = curNode->nextBlock;
			//smallestDifData = nextBlock;
			head = curNode->block;
			printf("%d -> ", curNode->block);
		}
		printf("DONE\n");
		
		dequeue(smallestDifData);
		count--;
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
		head = closest;
		
		printf("Path for file %d: ", closest);
		int nextBlock = closest;
		while(nextBlock != -1) {
			struct diskNode* curNode = diskFront;
			while(curNode->block != nextBlock) {
				curNode = curNode->next;
			}
				
			seek += abs(head - curNode->block);
			nextBlock = curNode->nextBlock;
			head = curNode->block;
			printf("%d -> ", curNode->block);
		}
		printf("DONE\n");
		
		head = closest; // Moves the head back to where the file started, so the next file request can be read using the scan algo.
		seek += abs(head-closest);
		dequeue(closest);
		count--;
		
		
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
		
		printf("Path for file %d: ", closest);
		int nextBlock = closest;
		while(nextBlock != -1) {
			struct diskNode* curNode = diskFront;
			while(curNode->block != nextBlock) {
				curNode = curNode->next;
			}
				
			seek += abs(head - curNode->block);
			nextBlock = curNode->nextBlock;
			head = curNode->block;
			printf("%d -> ", curNode->block);
		}
		printf("DONE\n");
		
		head = closest; // Moves the head back to where the file started, so the next file request can be read using the scan algo.
		seek += abs(head-closest);
		
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
		head = closest;
		
		printf("Path for file %d: ", closest);
		int nextBlock = closest;
		while(nextBlock != -1) {
			struct diskNode* curNode = diskFront;
			while(curNode->block != nextBlock) {
				curNode = curNode->next;
			}
				
			seek += abs(head - curNode->block);
			nextBlock = curNode->nextBlock;
			head = curNode->block;
			printf("%d -> ", curNode->block);
		}
		printf("DONE\n");
		
		head = closest; // Moves the head back to where the file started, so the next file request can be read using the scan algo.
		seek += abs(head-closest);
		
		dequeue(closest);
		count--;
		
		
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
		
		printf("Path for file %d: ", closest);
		int nextBlock = closest;
		while(nextBlock != -1) {
			struct diskNode* curNode = diskFront;
			while(curNode->block != nextBlock) {
				curNode = curNode->next;
			}
				
			seek += abs(head - curNode->block);
			nextBlock = curNode->nextBlock;
			head = curNode->block;
			printf("%d -> ", curNode->block);
		}
		printf("DONE\n");
		
		head = closest; // Moves the head back to where the file started, so the next file request can be read using the scan algo.
		seek += abs(head-closest);
		
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
				//break;
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
				break;
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

void initDisk() {
	int blockIn, nextIn, done = 0;
	while(done == 0) {
		if(fscanf(disk, "%d %d", &blockIn, &nextIn) == 2) {
			diskAdd(blockIn, nextIn);
		}
		else {
			done = 1;
		}
	}
}

void diskAdd(int blockIn, int nextIn) {
	struct diskNode* new = (struct diskNode*)malloc(sizeof(struct diskNode));
	new->block = blockIn;
	new->nextBlock = nextIn;
	new->next = NULL;
	
	if(diskFront == NULL && diskEnd == NULL) {
		diskFront = diskEnd = new;
	}
	else {
		struct diskNode* temp = diskFront;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new;
	}
}
