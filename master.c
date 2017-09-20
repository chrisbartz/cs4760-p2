#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include "restart.h"

#define CHILD_PROCESS "palin"
#define NUM_PROCESSES 2

pid_t r_wait(int *stat_loc);

int main(int argc, char *argv[]) {
   pid_t childpid;

   char* c_argv[strlen(CHILD_PROCESS)];
   strcpy(c_argv, CHILD_PROCESS);
   printf("%s\n",c_argv);

   if (argc < 1){      /* check for valid number of command-line arguments */
      fprintf (stderr, "Usage: %s command arg1 arg2 ...\n", argv[0]);
      return 1;
   }
   for (int i = 0; i < NUM_PROCESSES; i++) {
	    printf("fork: %d\n", i);
		childpid = fork();
		if (childpid == -1) {
			perror("Failed to fork");
			return 1;
		}
		if (childpid == 0) { /* child code */
			char str[1];
			sprintf(str, "%d", i);
			execvp(*c_argv, argv);
			perror("Child failed to execvp the command");
			return 1;
		}
		if (childpid != r_wait(NULL)) { /* parent code */
			perror("Parent failed to wait");
			return 1;
		}
   }

   return 0;
}

pid_t r_wait(int *stat_loc) {
   pid_t retval;
   while (((retval = wait(stat_loc)) == -1) && (errno == EINTR)) ;
   return retval;
}
