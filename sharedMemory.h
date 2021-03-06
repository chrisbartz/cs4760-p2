//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

char* create_shared_memory(int isParent);

char* detatch_shared_memory(char* shmpnt);

int write_shared_memory(char* memory, char* newdata);

void write_control(char* sharedMemory, char* entering, char* locked);

void read_control(char* sharedMemory, char* entering, char* locked);

void destroy_shared_memory();

#endif /* SHAREDMEMORY_H_ */
