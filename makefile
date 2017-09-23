all: master palin  

%.o: %.c 
	$(CC) -c -std=gnu99 $<

master: master.o sharedMemory.o timestamp.o
	gcc -o master master.o sharedMemory.o timestamp.o -lm /lib/libm.so.6 
	
palin: palin.o sharedMemory.o timestamp.o
	gcc -o palin palin.o sharedMemory.o timestamp.o -lm /lib/libm.so.6 

clean:
	rm master palin *.o 