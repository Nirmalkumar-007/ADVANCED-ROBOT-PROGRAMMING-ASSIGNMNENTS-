#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int up = 1;
int down = 2;
int left = 3;
int right = 4;
int brake_x = 5;
int brake_z = 6;
int flag = 0;
void handler(int sig)  // This function handles the arrival of the SIGUSR1 and SIGUSR2 signals.
{
	if (sig == SIGUSR1)
	{
		printf("\nCommands disabled \n");
		flag = 1; 
	}
	if (sig == SIGUSR2)
	{
		printf("\nCommands re-enabled\n");
		flag = 0; 
	}
	if (sig == SIGINT)
	{
		kill(getppid(), SIGTERM);
		kill(getpid(), SIGTERM);
	}
}

int main(int argc, char *argv[])
{

	int input, fd_mx, fd_mz, fd_insp;
	pid_t my_pid;
	my_pid = getpid();
	pid_t pid_wd = atoi(argv[1]);
	struct sigaction sa; // initialization of sigaction struct for signal handling
	sa.sa_handler = &handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	if((fd_mx = open(argv[2], O_WRONLY)) < 0){
        perror("error opening cmd x");
        return 1;
    }
	if((fd_mz = open(argv[3], O_WRONLY)) < 0 ){
        perror("error opening cmd z");
        return 1;
    }
	if((fd_insp = open(argv[4], O_WRONLY)) < 0){
        perror("error opening cmd z");
        return 1;
    }
	write(fd_insp, &my_pid, sizeof(int));
	printf("\nLeft Arrow: X axis decrease\n" );
	printf("\nRight Arrow: X axis increase\n" );
	printf("\nDown Arrow: Z axis decrease\n");
	printf("\nUp Arrow: Z axis increase\n");
	printf("\nx: X axis stop\n" );
	printf("\nz: Z axis stop\n"  );
	fflush(stdout);

	while ((input = getchar()) != 'e')
	{
		switch (flag) 
		{

		case 1:
			printf("\n System is resetting, please wait\n" );
			fflush(stdout);
			break;
			switch (input)
			{
			case 'x':
				printf("\n STOP MOTOR X\n" );
				fflush(stdout);
				write(fd_mx, &brake_x, sizeof(int));
				kill(pid_wd, SIGUSR1);
				break;
			case 'z':
				printf("\n STOP MOTOR Z\n" );
				fflush(stdout);
				kill(pid_wd, SIGUSR1);
				write(fd_mz, &brake_z, sizeof(int)); 
				break;
			case '\033': 
				getchar(); 

				switch (getchar()) 
				{

				case 'A': 
					
					printf("\n UP\n" );
					fflush(stdout);
					kill(pid_wd, SIGUSR1);
					write(fd_mz, &up, sizeof(int));
					break;

					
				case 'B':
					printf("\n DOWN\n" );
					fflush(stdout);
					kill(pid_wd, SIGUSR1);
					write(fd_mz, &down, sizeof(int));
					break;

				case 'C':
					printf("\n RIGHT\n" );
					fflush(stdout);
					kill(pid_wd, SIGUSR1);
					write(fd_mx, &right, sizeof(int));
					break;


				case 'D':
					printf("\n LEFT\n");
					fflush(stdout);
					kill(pid_wd, SIGUSR1);
					write(fd_mx, &left, sizeof(int)); 
					break;
				}
				break;
			}
			break;
		}
	}
	close(fd_mx);
	close(fd_mz);
	return 0;
}
