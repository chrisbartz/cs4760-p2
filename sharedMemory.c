//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

//This was an example on stack overflow
//https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define DEBUG 0
#define SHMKEY 12345
#define SHMSIZE 512

int shmid;
char* sharedMessage;

char* create_shared_memory(int isParent) {

	if (isParent) {
		if (DEBUG) printf("sharedMemory: Creating shared memory segment\n");
		if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0660)) == -1) {
			fprintf(stderr, "sharedMemory: shmget error code: %d", errno);
			perror("sharedMemory: Creating shared memory segment failed\n");
			exit(1);
		}
	} else {
		if (DEBUG) printf("sharedMemory: Opening shared memory segment\n");
		if ((shmid = shmget(SHMKEY, SHMSIZE, 0660)) == -1) {
			fprintf(stderr, "sharedMemory: shmget error code: %d", errno);
			perror("sharedMemory: Opening shared memory segment failed\n");
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
	if (DEBUG) printf("sharedMemory: write message: %s\n", message);
	write_shared_memory(sharedMemory, message);
	entering = NULL;
	locked = NULL;

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

	if (DEBUG) printf("sharedMemory: read entering:locked %s:%s\n", entering, locked);

}

void destroy_shared_memory() {

	if (DEBUG) printf("sharedMemory: Destroying shared memory segment\n");
	shmctl(shmid, IPC_RMID, NULL);

}
