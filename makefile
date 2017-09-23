all: master palin  

%.o: %.c 
	$(CC) -c -std=gnu99 $<

master: master.o sharedMemory.o timestamp.o
	gcc -o master master.o sharedMemory.o timestamp.o  
	
palin: palin.o sharedMemory.o timestamp.o
	gcc -o palin palin.o sharedMemory.o timestamp.o  

clean:
	rm master palin *.o 