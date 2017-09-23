//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void* create_shared_memory(size_t size);

#endif /* SHAREDMEMORY_H_ */
