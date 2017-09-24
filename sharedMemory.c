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
#define SHMSIZE 1024

int shmid;

char* create_shared_memory() {

	if (DEBUG) printf("sharedMemory: Creating shared memory segment\n");
	if ((shmid = shmget(SHMKEY,SHMSIZE,IPC_CREAT)) == -1){
		perror("sharedMemory: Creating shared memory segment failed\n");
		exit(1);
	}
	return shmat(shmid,NULL,0);

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
