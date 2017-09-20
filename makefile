all: my_prog   

%.o: %.c 
	$(CC) -c -std=gnu99 $<

my_prog: main.o  
	gcc -o my_prog main.o  

clean:
	rm my_prog* *.o