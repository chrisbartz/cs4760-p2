//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sharedMemory.h"
#include "timestamp.h"

#define DEBUG 1
#define CHILD_PROCESS "palin"
#define NUM_CHILD_PROCESSES_TO_SPAWN 5
#define MAX_CONCURRENT_CHILD_PROCESSES 2

pid_t r_wait(int *stat_loc);

int main(int argc, char *argv[]) {
	int childProcessCount = 0;
	pid_t childpid;
	//char *c_argv[2];
	char timeVal[30];

	getTime(timeVal);
	printf("master %s: create shared memory\n", timeVal);
	char* sharedMemory = create_shared_memory();

	char message[] = "Hello children!";
//	if (write_shared_memory(sharedMemory,message)) {
//		printf("write successful");
//	}

	if (argc < 1) { /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s command arg1 arg2 ...\n", argv[0]);
		return 1;
	}
	int i = 0;

	while (1) {
		getTime(timeVal);
		if (DEBUG) printf("master %s: Child processes count: %d\n", timeVal, childProcessCount);
		if (i >= NUM_CHILD_PROCESSES_TO_SPAWN)
			break;
		int status;
		if (wait(&status) >= 0) {
			getTime(timeVal);
			if (DEBUG) printf("master %s: Child process exited with %d status\n\n", timeVal, WEXITSTATUS(status));
			childProcessCount--; //because a child process completed
		}

		if (childProcessCount >= MAX_CONCURRENT_CHILD_PROCESSES) {
			getTime(timeVal);
			if (DEBUG) printf("master %s: Maximum child processes (%d) reached.  Sleeping 1 second\n", timeVal, childProcessCount);
			sleep(1);
			continue;
		}

		char iStr[1];
		sprintf(iStr, "%d", i);

		childpid = fork();

		if (childpid == -1) { //if error creating fork
			perror("master: Failed to fork");
			return 1;
		}

		if (childpid == 0) { // child will execute
			getTime(timeVal);
			if (DEBUG) printf("master %s: Child %d (fork #%d from parent) will attempt to execl palin\n", timeVal, (int) getppid(), i);
			execl("./palin", iStr, NULL);
			perror("master: Child failed to execl() the command");
			return 1;
		}

		if (childpid != 0) { //parent will execute
			childProcessCount++; //because we forked above

			getTime(timeVal);
			if (DEBUG) printf("master %s: parent forked child %d = childPid: %d\n", timeVal, i, (int) childpid);
		}

		i++;

	} //end while loop

//		if (childpid != r_wait(NULL)) { /* parent code */
//			perror("Parent failed to wait");
//			return 1;
//		}

//		int status;
//		if (wait(&status) >= 0) {
//			if (WIFEXITED(status)) {
//				/* Child process exited normally, through `return` or `exit` */
//				printf("Child process exited with %d status\n",
//						WEXITSTATUS(status));
//			}
//			if (WIFSIGNALED(status)) {
//				if (WCOREDUMP(status)) {
//					printf("Child process core dumped with %d signal\n",
//							WTERMSIG(status));
//				} else {
//					printf("Child process signal terminated with %d signal\n",
//							WTERMSIG(status));
//				}
//			}
//
//		}

	detatch_shared_memory(sharedMemory);
	printf("master: parent terminated normally\n");
	return 0;
}

pid_t r_wait(int *stat_loc) {
	pid_t retval;
	while (((retval = wait(stat_loc)) == -1) && (errno == EINTR))
		;
	return retval;
}
