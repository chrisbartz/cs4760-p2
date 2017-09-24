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

	if (DEBUG) printf("sharedMemory: Writing to shared memory segment\n");
	strncpy(sharedMemory, newdata, SHMSIZE);
	return 1;

}
