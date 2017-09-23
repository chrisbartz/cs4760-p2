//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>


#define SLEEP_INTERVAL 2

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

if (childId < 0) {
	fprintf(stderr, "palin: Something wrong with child id: %d\n", childId);
	exit(1);
} else {
	fprintf(stdout, "palin: Child %d started normally\n", (int) getpid());
	sleep(SLEEP_INTERVAL);
	fprintf(stdout, "palin: Child %d exiting normally\n", (int) getpid());
}
exit(0);
}
