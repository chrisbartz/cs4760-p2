//Christopher Bartz
//cyb01b
//CS4760 S02
//Project 2

//taken from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>

#define DEBUG 0

void getTime(char* buffer) {
//	 char buffer[26];
	  int millisec;
	  struct tm* tm_info;
	  struct timeval tv;

	  gettimeofday(&tv, NULL);

	  millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
	  if (millisec>=1000) { // Allow for rounding up to nearest second
	    millisec -=1000;
	    tv.tv_sec++;
	  }

	  tm_info = localtime(&tv.tv_sec);

	  strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
	if (DEBUG) printf("timestamp: %s.%03d\n", buffer, millisec);
}
