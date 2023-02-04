#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
int flag = 0; 
void handler(int sig)
{
	if (sig == SIGUSR1)
	{
		flag = 1; 
	}
	
	if (sig == SIGUSR2)
	{
		flag = 2;
	}
}

int main(int argc, char *argv[])
{
	fd_set rset;
	struct timeval tv = {0L, 0L};
	int retval, cmd, step, max, err, pos_x, est_pos_x, fd_x_in, fd_x_out, max_step, max_err;
	pid_t my_pid;
	max_err = 4; 
	step = 20; 
	max_step = max_err + step; 
	max = 10000; 
	pos_x = 0;
	est_pos_x = 0; 
	my_pid = getpid();
	cmd = 5;

	struct sigaction sa;
	sa.sa_handler = &handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);

	if((fd_x_in = open(argv[1], O_RDONLY)) < 0){
        perror("MOTOR X: Error opening fd_x_in ");
        return 1;
    }
	if((fd_x_out = open(argv[2], O_WRONLY)) < 0){
        perror("MOTOR X: Error opening fd_x_out ");
        return 1;
    }
	write(fd_x_out, &my_pid, sizeof(int));
	while (1)
	{
		FD_ZERO(&rset);
		FD_SET(fd_x_in, &rset);
		retval = select(FD_SETSIZE, &rset, NULL, NULL, &tv);
		fflush(stdout);
		if (retval == -1)
		{
			perror("select()");
			exit(-1);
		}
		else if (retval > 0)
		{

			if (FD_ISSET(fd_x_in, &rset) > 0)
			{
		        read(fd_x_in, &cmd, sizeof(int));
			}
		}
		srand(2);
		err = (rand() % (max_err + 1)) - (rand() % (max_err + 1));
		switch (flag)
		{	
		case 0:
			if (cmd != 5)
			{
				if (cmd == 3 && est_pos_x > max_step)
				{ 
					pos_x -= step;
					est_pos_x -= step + err;
					if (est_pos_x <= 25)
					{
						pos_x = 0;
						est_pos_x = 0;
					}

					write(fd_x_out, &est_pos_x, sizeof(float));
				}

				if (cmd == 4 && est_pos_x < max - max_step)
				{ 
					pos_x += step;
					est_pos_x += step + err;

					if (est_pos_x >= 9975)
					{
						pos_x = 10000;
						est_pos_x = 10000;
					}

					write(fd_x_out, &est_pos_x, sizeof(float));
				}
			}
			break;
		case 1:
			if (est_pos_x >= max_step)
			{
				pos_x -= step;
				est_pos_x -= step + err;
				if (est_pos_x <= 25)
				{
					pos_x = 0;
					est_pos_x = 0;
				}
			}
			write(fd_x_out, &est_pos_x, sizeof(float));
			if (est_pos_x < max_step)
			{
				flag = 2;
			}
			break;	
		case 2:
			cmd = 5;
			flag = 0;
			break;
		}
		usleep(200000);
	}
	close(fd_x_in);
	close(fd_x_out);
	return 0;
}
