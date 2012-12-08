# this is for arming
all:	arming

arming:	dbApi.o main.o
	gcc -o $@ $^ -ltokyocabinet

.c.o:
	gcc -c $<

clean:
	rm *.o arming
