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

#define DEBUG 1
#define SLEEP_INTERVAL 2

int solve_palindrome(char palin[]);

//extern bool choosing[n]; /* Shared Boolean array */
//extern int number[n]; /* Shared integer array to hold turn number */
//void process_i ( const int i ) /* ith Process */ {
// do
// choosing[i] = true;
// number[i] = 1 + max(number[0], ..., number[n-1]);
// choosing[i] = false;
// for ( int j = 0; j < n; j++ ) {
// while ( choosing[j] ); // Wait if j happens to be choosing
// while ( (number[j] != 0)
// && ( number[j] < number[i] || (number[j] == number[i] && j < i) );
// }
// critical_section();
//
// number[i] = 0;
// remainder_section();
// while ( 1 );
//}



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
	int isPalindrome = solve_palindrome(palin);

	getTime(timeVal);
	if (DEBUG) if (isPalindrome)
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


