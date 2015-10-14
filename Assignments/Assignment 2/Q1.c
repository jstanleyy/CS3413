// CS3413 - Operating Systems I
// Assignment 2 - Question 1
// Joel Stanley
// Due October 15, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Used in the linked list queue.
struct Node {
	char *user;
	char process;
	int arrival;
	int duration;
	struct Node *next;
	struct Node *prev;
};

// Used to keep track of the last job each person finished.
struct Person {
	char *name;
	int last;
};

//Pointers for Queue.
struct Node* front = NULL;
struct Node* end = NULL;

FILE *file;

struct Person info[10];
int n;
int curTime = 0, count = 0, k = 0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;


// Adds the Node structure to the end of the queue.
void enqueue(char *userIn, char processIn, int arrivalIn, int durationIn) {
	struct Node* new = (struct Node*)malloc(sizeof(struct Node));
	new->user = userIn;
	new->process = processIn;
	new->arrival = arrivalIn;
	new->duration = durationIn;
	new->next = NULL;
	new->prev = NULL;

	if(front == NULL && end == NULL) {
		front = end = new;
	}
	else {
		struct Node* check = front;
		int i = 0;
	 	printf("%d, %d\n", count, i);
		for(i = 0; i <= count + 1; i++ ) {
			printf("check: %d, %d\n", check->duration, new->duration);
			if(new->duration == check->duration) {
				// Add 1 behind
				if(check == end) {
					check->next = new;
					new->prev = check;
				}
				else {
					struct Node* temp = check->next;
					printf("here2");
					check->next = new;
					new->next = temp;
					new->prev = check;
					temp->prev = new;
					printf("here2");
					break;
				}
			}
			else if (new->duration < check->duration) {
				// Add 1 before
				if(check == front) {
					new->next = front;
					front->prev = new;
					front = new;
				}
				else {
					struct Node* temp = check->prev;
					temp->next = new;
					new->next = check;
					new->prev = check->prev;
					check->prev = new;
				}
				break;
			}
			else if (check->next == NULL) {
				// Add to end
				check->next = new;
				new->prev = check;
				end = new;
			}
			else {	
				check = check->next;
			}
		}
//		end->next = new;
//		end = new;
	}
}
 
void * run(void *in) {
	int core = *((int *) in);
	int timeInCore = curTime;
	for( ; ; ) {
		if (front != end) {
		pthread_mutex_lock(&m1);
		if(timeInCore >= front->arrival) {
			printf("%d	%c\n", timeInCore, front->process);
			int sleepTime = front->duration;
			k++;
			pthread_mutex_unlock(&m1);
			//Keeps track of the last job for each user.
			int j;
			for(j = 0; j < count; j++) {
				if(strcmp(info[j].name, front->user) == 0) {
					info[j].last = timeInCore + front->duration;
					break;
				}
			}
			front = front->next;
			
     			sleep(sleepTime);
			timeInCore += sleepTime;
		}
		else {
			timeInCore++;
		}
		}
	}
	printf("%d	CPU %d IDLE\n", timeInCore, core);
}

int main(int argc, char *argv[]) {
	
	// File I/O variables.
	file = fopen("data.txt", "r");
	char line[1024];
	memset(line, 0, 1024);
	
	// Variables used throughout the function.
	char tProcess;
	int tArrival;
	int tDuration;
	int done = 0;
	int i, skip = 0, j, dup = 0;
	n = *argv[1] - 48;

	pthread_t thread_id[n];
	for(i = 0; i < n; i++) {
		int *coreNum = malloc(sizeof(*coreNum));
		*coreNum = i + 1;
		pthread_create(&thread_id[i], NULL, &run, coreNum);
	}
	
	fgets(line, 1024, file); // Skips the first line of the input.
	
	while(done == 0) {
		char *tUser = (char *)malloc(sizeof(char[20])); // Allocates memory to be used for the input names
		if(fscanf(file,"%s	%c	%d	%d", tUser, &tProcess, &tArrival, &tDuration) == 4) {
			printf("adding user %s\n", tUser);
			enqueue(tUser, tProcess, tArrival, tDuration);
			printf("added %s\n", tUser);	
			// Checks if the name already exists in the array, and skips if so.
			for(i = 0; i < count; i++) {
				if(strcmp(info[i].name, tUser) == 0) { 
					skip = 1;
					dup++;
				}
			}

			// Adds the person to the array if not already there.
			if(skip == 0) {
				struct Person person;
				person.name = tUser;
				info[count] = person;
			}

			count = count + 1; // Keeps track of how many lines of input have been read.
		}
		else {
			done = 1;
		}
	}
	
	fclose(file); // Closes the file as it is no longer needed.

	curTime = 0;

	printf("Time	Job\n");
	
	int timeDone[n]; // Used to keep track of cores.

	for(i = 0; i < n; i++) {
		timeDone[i] = 0;
	}
	
	for(i = 0; i< n; i++) {
    	pthread_join(thread_id[i], NULL);
	}

	// Prints each process that is executing on each CPU.
	/*while(k < count) {
		for(i = 0; i < n; i++) { // Loops for each CPU.
			if(time >= front->arrival) { // Can only proceed if the next task has arrived.
				if(time >= timeDone[i]) {
					pthread_t thread;
					pthread_create(&thread, NULL, &run, NULL);
					pthread_join(thread, NULL);
					
					printf("%d	%c\n", time, front->process);
					timeDone[i] = time + front->duration; // Updates when that CPU will be free.
				
					//Keeps track of the last job for each user.
					for(j = 0; j < count; j++) {
							if(strcmp(info[j].name, front->user) == 0) {
							info[j].last = time + front->duration;
							break;
						}
					}
					
					front = front->next;
					k++;
				}
			} 

			if (k >= count) {
				break;
			}

		}

	time++;

	} */
		
	// Prints the Idle times for each CPU.
	//for(i = 0; i < n; i++) {
		//printf("%d	CPU %d IDLE\n", curTime, i + 1);
	//}
	
	// Prints the Summary.
	printf("\nSummary\n");

	for(i = 0; i < (count-dup); i++) {
		printf("%s	%d\n", info[i].name, info[i].last);
	}	

	return 0;
}
