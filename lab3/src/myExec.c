#include <stdio.h>
#include <unistd.h>

int main( void ) {
	char *argv[3] = {"Command-line", ".", NULL};

	int pid = fork();

	if ( pid == 0 ) {
		execl("myEx3","myEx3","1234","100", NULL);
	}

	printf( "Sequential... is executing\n" );

	return 0;
}