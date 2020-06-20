CC=gcc -std=c99
CFLAGS=-I.
 
all : tcp udp
 
tcp : tcpclient tcpserver

udp : udpclient udpserver

tcpserver :
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)

tcpclient :
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)

udpserver :
	$(CC) -o udpserver udpserver.c $(CFLAGS)

udpclient :
	$(CC) -o udpclient udpclient.c $(CFLAGS)
 
clean :
	rm tcpserver tcpclient udpserver udpclient
