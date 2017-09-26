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
#define NUM_CHILD_PROCESSES_TO_SPAWN 8
#define MAX_CONCURRENT_CHILD_PROCESSES 3

int main(int argc, char *argv[]) {
	int childProcessCount = 0;
	int opt;
	pid_t childpid;
	char timeVal[30];
	char filename[30];

//	//gather option flags
//	while ((opt = getopt(argc, argv, "n:k:m:hwc:")) != -1) {
//		switch (opt) {
//		case 'f':
//			filename = optarg;
//			if (DEBUG) printf("opt f detected: %sd\n", filename);
//			break;
//		case 'h':
//			if (DEBUG) printf("opt h detected\n");
//			return printHelp(argv, 0);
//			break;
//		default:
//			break;
//		}
//	}


	getTime(timeVal);
	if (DEBUG) printf("\n\nmaster %s: create shared memory\n", timeVal);
	char* sharedMemory = create_shared_memory(1);

		// simple message example
	//	char message[] = "Hello children!";
	//	if (write_shared_memory(sharedMemory, message)) {
	//		getTime(timeVal);
	//		if (DEBUG) printf("master %s: write successful\n", timeVal);
	//	}

	// more complicated messaging
	char* entering[NUM_CHILD_PROCESSES_TO_SPAWN + 1];
	char* locked[NUM_CHILD_PROCESSES_TO_SPAWN + 1];

	for (int k = 0; k < NUM_CHILD_PROCESSES_TO_SPAWN; k++){
		strncpy(entering,"00000000",NUM_CHILD_PROCESSES_TO_SPAWN + 1);
		strncpy(locked,"11111111",NUM_CHILD_PROCESSES_TO_SPAWN + 1);
	}
//	entering[NUM_CHILD_PROCESSES_TO_SPAWN + 1] = (char) '\0';
//	locked[NUM_CHILD_PROCESSES_TO_SPAWN + 1] = (char) '\0';
	printf("entering %s\n",entering);
	printf("locked %s\n",locked);

	write_control(sharedMemory, entering, locked);
	getTime(timeVal);
	if (DEBUG) printf("master %s: write successful: %s\n", timeVal, sharedMemory);


	if (argc < 1) { /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s command arg1 arg2 ...\n", argv[0]);
		return 1;
	}
	int i = 0;

	while (1) {
		getTime(timeVal);
		if (DEBUG) printf("master %s: Child processes count: %d\n", timeVal, childProcessCount);
		if (i >= NUM_CHILD_PROCESSES_TO_SPAWN) {
			while (childProcessCount > 0) {

				int status;
				if (wait(&status) >= 0) {
					getTime(timeVal);
					printf("master %s: Child process exited with %d status\n", timeVal, WEXITSTATUS(status));
					childProcessCount--; //because a child process completed
				}

				if (DEBUG) printf("master %s: Child processes count: %d\n", timeVal, childProcessCount);
			}
			break;
		}


		if (childProcessCount >= MAX_CONCURRENT_CHILD_PROCESSES) {
			getTime(timeVal);
			printf("master %s: Maximum child processes (%d) reached.  Currently on %d of %d.  Waiting for a child to terminate\n", timeVal, childProcessCount, i + 1, NUM_CHILD_PROCESSES_TO_SPAWN);

			int status;
			if (wait(&status) >= 0) {
				getTime(timeVal);
				printf("master %s: Child process exited with %d status\n", timeVal, WEXITSTATUS(status));
				childProcessCount--; //because a child process completed
			}

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

	detatch_shared_memory(sharedMemory);
	destroy_shared_memory();
	printf("master: parent terminated normally\n\n");
	return 0;
}

