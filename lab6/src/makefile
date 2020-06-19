CC=gcc -std=c99
CFLAGS=-I.
 
all : client server
 
client : multmodulo
    $(CC) -pthread -L -lmultm -o client client.c $(CFLAGS)
 
server : multmodulo
    $(CC) -pthread -L -lmultm -o server server.c $(CFLAGS)
 
multmodulo :
    $(CC) -c -o multmodulo.o multmodulo.c && ar rcs libmultm.a multmodulo.o
 
clean :
    rm client server multmodulo
