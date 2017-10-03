//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include "sharedMemory.h"
#include "timestamp.h"

#define DEBUG 0
#define SLEEP_INTERVAL 2

int isPalindrome;

int solve_palindrome(char palin[]);
void critical_section(char palin[]);


int main(int argc, char *argv[]) {
int childId = atoi(argv[0]);
char timeVal[30];
srand(time(NULL));

getTime(timeVal);
if (childId < 0) {
	if (DEBUG) fprintf(stderr, "palin  %s: Something wrong with child id: %d\n", timeVal, getpid());
	exit(1);
} else {
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d started normally after execl\n", timeVal, (int) getpid());

	char palin[100];
	strncpy(palin, argv[1], 100);
	getTime(timeVal);
	fprintf(stdout, "palin  %s: Child %d found a palindrome to solve: %s\n", timeVal, (int) getpid(), palin);

	// solve paindrome
	isPalindrome = solve_palindrome(palin);

	getTime(timeVal);
	if (isPalindrome)
		fprintf(stdout, "palin  %s: Child %d found \"%s\" is a palindrome\n", timeVal, (int) getpid(), palin);
	else
		fprintf(stdout, "palin  %s: Child %d found \"%s\" is NOT a palindrome\n", timeVal, (int) getpid(), palin);

	char* sharedMemory = create_shared_memory(0);

//	char* entering;
//	char* locked;
//	read_control(sharedMemory, entering, locked);
//	if (DEBUG) fprintf(stdout, "palin  %s: Child %d read shared memory: %s:%s\n", timeVal, (int) getpid(), entering, locked);

	getTime(timeVal);
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d read shared memory: %s\n", timeVal, (int) getpid(), sharedMemory);
//
//	char message[] = "Hello everybody from child ";
//	char id[8];
//	sprintf(id, "%d", (int) getpid());
//	strncat(message, id, sizeof(message));
//	write_shared_memory(sharedMemory, message);

	int forAWhile = (rand() % SLEEP_INTERVAL) + 1;
	getTime(timeVal);
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d sleeping for %d seconds\n", timeVal, (int) getpid(), forAWhile);
	sleep(forAWhile);

	// critical section
	//this is where the multiple processor solution is supposed to be implemented
	int flag[500];
	int idle = 0;
	int want_in = 1;
	int in_cs = 3;
	int i = childId;
	int j, n, turn;

	//n = number_of_processes;

//	do {
//			do {
//				flag[i] = want_in; // Raise my flag
//				j = turn; // Set local variable
//				// wait until its my turn
//				while (j != i)
//					j = (flag[j] != idle) ? turn : (j + 1) % n;
//				// Declare intention to enter critical section
//				flag[i] = in_cs;
//				// Check that no one else is in critical section
//				for (j = 0; j < n; j++)
//					if ((j != i) && (flag[j] == in_cs))
//						break;
//			} while ((j < n) || (turn != i && flag[turn] != idle));
//			// Assign turn to self and enter critical section
//			turn = i;
			critical_section(palin);
//			// Exit section
//			j = (turn + 1) % n;
//			while (flag[j] == idle)
//				j = (j + 1) % n;
//			// Assign turn to next waiting process; change own flag to idle
//			turn = j;
//			flag[i] = idle;
//			remainder_section();
//		} while (1);



	// end critical section


	detatch_shared_memory(sharedMemory);
	getTime(timeVal);
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d exiting normally\n", timeVal, (int) getpid());
}
exit(0);
}

int solve_palindrome(char palin[]) {
	char timeVal[30];
	int lengthOfPalindrome = strlen(palin);
	getTime(timeVal);
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d evaluating palindrome - length: %d\n", timeVal, (int) getpid(), lengthOfPalindrome);

	for (int i = 0; i < lengthOfPalindrome/2; i++) {
		getTime(timeVal);
		char leading = palin[i];
		char tailing = palin[lengthOfPalindrome - i - 1];
		getTime(timeVal);
		if (DEBUG) fprintf(stdout, "palin  %s: Child %d evaluating palindrome - i: %d l: %c t: %c\n", timeVal, (int) getpid(), i, leading, tailing);
		if (leading != tailing) {
			return 0;
		}
	}
	return 1;
}

void critical_section(char palin[]) {
	char timeVal[30];
	getTime(timeVal);
	fprintf(stdout, "palin  %s: Child %d entering CRITICAL SECTION\n", timeVal, (int) getpid());
	if (DEBUG) fprintf(stdout, "palin  %s: Child %d opening file\n", timeVal, (int) getpid());
	FILE *file;
	if (isPalindrome) {
		file = fopen("palin.out", "a");
	} else {
		file = fopen("nopalin.out", "a");
	}
	//sleep(forAWhile);
	fprintf(file, "%s\n", palin);
	//sleep(forAWhile);
	fclose(file);
	getTime(timeVal);
	fprintf(stdout, "palin  %s: Child %d exiting CRITICAL SECTION\n", timeVal, (int) getpid());
}
