//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

//This was an example on stack overflow
//https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define DEBUG 1
#define SHMKEY 12345
#define SHMSIZE 512

int shmid;

char* create_shared_memory(int isParent) {

	if (isParent) {
		if (DEBUG) printf("sharedMemory: Creating shared memory segment\n");
		if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT)) == -1) {
			perror("sharedMemory: Creating shared memory segment failed - you might have to use sudo to execute!\n");
			exit(1);
		}
	} else {
		if (DEBUG) printf("sharedMemory: Opening shared memory segment\n");
		if ((shmid = shmget(SHMKEY, SHMSIZE, 0)) == -1) {
			perror("sharedMemory: Opening shared memory segment failed - you might have to use sudo to execute!\n");
			exit(1);
		}
	}

	char* sharedMemory = shmat(shmid,NULL,0);
	if (DEBUG)printf("sharedMemory: Shared memory attached at address %p\n", sharedMemory);
	return sharedMemory;

}

char* detatch_shared_memory(char* shmpnt) {

	if (DEBUG) printf("sharedMemory: Detatching shared memory segment\n");
	shmdt(&shmpnt);

}

int write_shared_memory(char* sharedMemory, char* newdata) {

	if (DEBUG) printf("sharedMemory: Writing to shared memory segment: %s\n", newdata);
	strncpy(sharedMemory, newdata, SHMSIZE);
	return 1;

}

void write_control(char* sharedMemory, char* entering, char* locked) {

	char message[50];
	strncat(message, entering, sizeof(message));
	strncat(message, "|", sizeof(message));
	strncat(message, locked, sizeof(message));
	if (DEBUG) printf("sharedMemory: message: %s\n", message);
	write_shared_memory(sharedMemory, message);

}

void read_control(char* sharedMemory, char* entering, char* locked) {
	char *stringArray[2];
	char *p = strtok(sharedMemory, "|");
	int i = 0;
	while (p != NULL) {
		stringArray[i++] = p;
		p = strtok(NULL, " ");
	}

	entering = stringArray[0];
	locked = stringArray[1];
	if (DEBUG) printf("sharedMemory: entering: %s\n", entering);
	if (DEBUG) printf("sharedMemory: locked: %s\n", locked);

}
