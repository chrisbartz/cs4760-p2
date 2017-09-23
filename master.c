#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEBUG 1
#define CHILD_PROCESS "palin"
#define NUM_PROCESSES 2

pid_t r_wait(int *stat_loc);
char* getTime();

int main(int argc, char *argv[]) {
   pid_t childpid;

   char *c_argv[2];
   strcpy(&c_argv[0], CHILD_PROCESS);

   if (argc < 1){      /* check for valid number of command-line arguments */
      fprintf (stderr, "Usage: %s command arg1 arg2 ...\n", argv[0]);
      return 1;
   }
   for (int i = 0; i < NUM_PROCESSES; i++) {
		char iStr[1];
		sprintf(iStr, "%d", i);
		strcpy(&c_argv[1], iStr);
		//if (DEBUG) printf("%s:%s\n", &c_argv[0],&c_argv[1]);
		if (DEBUG) printf("parent forking: %d\n", i);
		childpid = fork();


		if (childpid == -1) {
			perror("Failed to fork");
			return 1;
		}
		if (childpid == 0) { /* child code */
			if (DEBUG) printf("ChildPid %d (fork %d) will attempt to execl\n", (int) childpid, i);
			execl("./palin", iStr, NULL);
			perror("Child failed to execl() the command");
			return 1;
		}
		if (DEBUG) printf("parent forked: %d = childPid: %d\n", i, (int) childpid);
		if (childpid != r_wait(NULL)) { /* parent code */
			perror("Parent failed to wait");
			return 1;
		}
		int status;
		if (wait(&status) >= 0) {
			if (WIFEXITED(status)) {
				/* Child process exited normally, through `return` or `exit` */
				printf("Child process exited with %d status\n",
						WEXITSTATUS(status));
			}
			if (WIFSIGNALED(status)) {
				if (WCOREDUMP(status)) {
					printf("Child process core dumped with %d signal\n",
							WTERMSIG(status));
				} else {
					printf("Child process signal terminated with %d signal\n",
							WTERMSIG(status));
				}
			}

		}
	}


   return 0;
}

pid_t r_wait(int *stat_loc) {
   pid_t retval;
   while (((retval = wait(stat_loc)) == -1) && (errno == EINTR)) ;
   return retval;
}

char* getTime() {
	time_t timer;
	char buffer[26];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);

	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	return buffer;
}
