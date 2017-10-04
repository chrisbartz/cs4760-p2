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
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sharedMemory.h"
#include "timestamp.h"

#define DEBUG 0 // setting to 1 greatly increases number of logging events
#define CHILD_PROCESS "palin"
//#define NUM_CHILD_PROCESSES_TO_SPAWN 8
#define MAX_CONCURRENT_CHILD_PROCESSES 19

int signalIntercepted = 0;

void trim_newline(char *string);
void signal_handler(int signalIntercepted);

int main(int argc, char *argv[]) {
	int childProcessCount = 0;
	int opt;
	pid_t childpid;
	char timeVal[30];
	char *filename;
	pid_t childpids[500];

	//gather option flags
	while ((opt = getopt(argc, argv, "f:h")) != -1) {
		switch (opt) {
		case 'f':
			filename = optarg;
			if (DEBUG) printf("opt f detected: %s\n", filename);
			break;
		case 'h':
			if (DEBUG) printf("opt h detected\n");
			fprintf(stderr,"Usage: ./master <arguments>\n");
			break;
		default:
			break;
		}
	}

	// open file and inspect contents
	char palinValues[500][100]; // palindrome values read into an array
	char row[100];				// temp holding spot for each row
	int palinValuesLength = 0;	// total count of palindromes read

	// read in specified file
	FILE *file;
	file = fopen(filename,"r");

	if (file == NULL) {
		perror("Cannot open file");
		exit(1);
	}

	// store in palinValues
	while (fgets(row , sizeof(row), file)) {
		trim_newline(row);
		strncpy(palinValues[palinValuesLength++], row, sizeof(row));
		if (DEBUG) puts(palinValues[palinValuesLength -1]);
	}

	// clean up file object
	fclose(file);

	// instantiate shared memory from master
	getTime(timeVal);
	if (DEBUG) printf("\n\nmaster %s: create shared memory\n", timeVal);
	char* sharedMemory = create_shared_memory(1);

	// attempt to insert all palin values into shared memory - not working properly
	if (DEBUG) for (int l = 0; l < palinValuesLength; l++) {
		if (DEBUG) printf("%s\n", palinValues[l]);
		// simple message example
		if (write_shared_memory(&sharedMemory[l], palinValues[l])) {
			getTime(timeVal);
			if (DEBUG) printf("master %s: write successful\n", timeVal);
		}
	}


	// an attempt at more complicated messaging - did not work
//	char entering[palinValuesLength + 1];
//	char locked[palinValuesLength + 1];
//
//	for (int k = 0; k < palinValuesLength; k++){
//		strncpy(entering,"00000000",palinValuesLength + 1);
//		strncpy(locked,"11111111",palinValuesLength + 1);
//	}
////	entering[NUM_CHILD_PROCESSES_TO_SPAWN + 1] = (char) '\0';
////	locked[NUM_CHILD_PROCESSES_TO_SPAWN + 1] = (char) '\0';
//	printf("entering %s\n",entering);
//	printf("locked %s\n",locked);

////	write_control(sharedMemory, entering, locked);
//	getTime(timeVal);
//	if (DEBUG) printf("master %s: write successful: %s\n", timeVal, sharedMemory);


	if (argc < 1) { /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s command arg1 arg2 ...\n", argv[0]);
		return 1;
	}
	int i = 0;

	//register signal handler
	signal(SIGINT, signal_handler);



	// this is the main loop
	while (1) {

		//what to do when signal encountered
		if (signalIntercepted) { // signalIntercepted is set by signal handler
			printf("\nmaster: //////////// parent terminating children due to a signal! //////////// \n\n");
			// kill all running child processes
			for (int p = 0; p < i; p++) {
				printf("master: //////////// parent terminating child process %d //////////// \n", (int) childpids[p]);
				kill(childpids[p], SIGTERM);
			}

			// close all open shared memory
			detatch_shared_memory(sharedMemory);
			destroy_shared_memory();
			printf("master: parent terminated due to a signal!\n\n");
			exit(130);
		}

		getTime(timeVal);
		if (DEBUG) printf("master %s: Child processes count: %d\n", timeVal, childProcessCount);

		// if total forked processes exceed the number of palindromes
		// then we wait for them to exit and then break out of the while loop
		if (i >= palinValuesLength) {
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

		// if we have forked up to the max concurrent child processes
		// then we wait for one to exit before forking another
		if (childProcessCount >= MAX_CONCURRENT_CHILD_PROCESSES) {
			getTime(timeVal);
			printf("master %s: Maximum child processes (%d) reached.  Currently on %d of %d.  Waiting for a child to terminate\n", timeVal, childProcessCount, i + 1, palinValuesLength);

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

		// if error creating fork
		if (childpid == -1) {
			perror("master: Failed to fork");
			return 1;
		}

		// child will execute
		if (childpid == 0) {
			getTime(timeVal);
			if (DEBUG) printf("master %s: Child (fork #%d from parent) will attempt to execl palin\n", timeVal, i);
			execl("./palin", iStr, palinValues[i], NULL);
			perror("master: Child failed to execl() the command");
			return 1;
		}

		// parent will execute
		if (childpid != 0) {
			childpids[i] = childpid; // save child pids in an array
			childProcessCount++; // because we forked above

			getTime(timeVal);
			if (DEBUG) printf("master %s: parent forked child %d = childPid: %d\n", timeVal, i, (int) childpid);
		}

		i++;

	} //end while loop

	// clean up
	detatch_shared_memory(sharedMemory);
	destroy_shared_memory();
	printf("master: parent terminated normally\n\n");
	return 0;
}

// remove newline characters from palinValues
void trim_newline(char *string) {
	string[strcspn(string, "\r\n")] = 0;
}

// handle the ^C interrupt
void signal_handler(int signal) {
	if (DEBUG) printf("\nmaster: //////////// Encountered signal! //////////// \n\n");
	signalIntercepted = 1;
}
