#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

struct sigaction newhandler, oldhandler;
sigset_t sig;

int pid[10];
int numProcs = 0;
int currentProc = 0;

void setblock( int fd ,bool block){

	static int blockf, nonblockf;
	static bool first = true;

	int flags;

	if(first){

		first = false;
		if((flags = fcntl(fd,F_GETFL,0)) == -1){

			fprintf(stderr," fcntl - error \n");
			exit(1);
		}

		blockf = flags & ~ O_NDELAY;
		nonblockf = flags | O_NDELAY;
	}

	if(fcntl(fd,F_SETFL, block ? blockf : nonblockf) == -1 ){

		fprintf(stderr," fcntl2 - error \n");
		exit(1);
	}
}

void next(int code){

	if(numProcs > 0){

		kill(pid[currentProc], SIGSTOP);

		currentProc++;

		if(currentProc == numProcs)
			currentProc = 0;

		kill(pid[currentProc], SIGCONT);
	}
}

void execute(char* message){

	pid[numProcs] = fork();

	if(pid[numProcs] == 0){
		execl("./writer", "writer", message, NULL);	//Child will execute writer
	}

	kill(pid[numProcs], SIGSTOP);
	
	numProcs++;	//Inc number of procedures
}

int main(){

	char mess[80];
	int fd;
	int numch;

 
	fd = open("/dev/tty",O_RDWR);
	setblock(fd,false);

	//Install signal handler for PARENT:
	sigemptyset(&sig);
	newhandler.sa_handler = next;
	newhandler.sa_mask = sig;
	newhandler.sa_flags = 0;

	if(sigaction(SIGALRM,&newhandler,&oldhandler) == -1 ){

		printf("1 - can't install signal handler \n");
		exit(-1);
	}


	while(1){

		switch (numch = read(fd,mess,sizeof(mess))){

		case -1 :
		case 0 :
				break;
		default:
			// Convert contents of mess into c-style string.
			mess[numch-1] = '\0';

			execute(mess);	//calls execute function, passing it mess, which acts as a buffer
		}

		alarm(3);
		pause();

		fprintf(stderr, "\n");
	}

	return 0;
}
