# this is for arming
TAEGETS = arming client server  
#\testprotocol testclient testserver

all:	arming client server

#remote client command line
client: socketwrapper.o protocol.o remoteDBApi.o serverNode.o distributeDBApi.o client.o 
	gcc -o $@ $^ -ltokyocabinet
#server
server: socketwrapper.o protocol.o dbApi.o serverNode.o server.o server_start.o
	gcc -g -o $@ $^ -ltokyocabinet
arming:	dbApi.o client.o
	gcc -o $@ $^ -ltokyocabinet

test: protocol.o testprotocol.o socketwrapper.o testsocketwrapperclient.o testsocketwrapperserver.o
	gcc -o testprotocol protocol.o testprotocol.o
	./testprotocol
	# gcc -o testserver socketwrapper.o testsocketwrapperserver.o
	# gcc -o testclient socketwrapper.o testsocketwrapperclient.o
	# ./testserver
	
	# ./testclient
.c.o:
	gcc -c $<

clean:
	rm *.o $(TAEGETS) 
