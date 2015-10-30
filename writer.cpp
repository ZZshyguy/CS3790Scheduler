#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

	char *message = argv[1];

	while(true){

		fprintf(stderr, "%s ", message);
		sleep(1);
	}

	return 0;
}

