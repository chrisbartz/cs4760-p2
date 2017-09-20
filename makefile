all: master palin  

%.o: %.c 
	$(CC) -c -std=gnu99 $<

master: master.o  
	gcc -o master master.o  
	
palin: palin.o  
	gcc -o palin palin.o

clean:
	rm master palin *.o 