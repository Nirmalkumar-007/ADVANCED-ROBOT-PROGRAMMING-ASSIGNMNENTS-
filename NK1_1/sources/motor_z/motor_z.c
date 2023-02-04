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
	int retval, cmd, step, max, err, est_pos_z, pos_z, fd_z_in, fd_z_out, max_step, max_err;
	pid_t my_pid;
	step = 20; 
	max_err = 4; 
	max_step = step + max_err; 
	max = 10000;
	est_pos_z = 0; 
	pos_z = 0; 
	my_pid = getpid(); 
	cmd = 6;
	struct sigaction sa;
	sa.sa_handler = &handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	if((fd_z_in = open(argv[1], O_RDONLY)) < 0){
		perror("MOTOR z: error opening fd_z_in");
		return 1;
	}
	if((fd_z_out = open(argv[2], O_WRONLY)) < 0){
		perror("MOTOR z: error opening fd_z_out");
		return 1;
	}
	write(fd_z_out, &my_pid, sizeof(int));
	while (1)
	{
		FD_ZERO(&rset);
		FD_SET(fd_z_in, &rset);
		retval = select(FD_SETSIZE, &rset, NULL, NULL, &tv);
		fflush(stdout);
		if (retval == -1)
		{
			perror("select()");
			exit(-1);
		}

		else if (retval > 0)
		{

			if (FD_ISSET(fd_z_in, &rset) > 0)
			{
				read(fd_z_in, &cmd, sizeof(int));
			}
		}

		err = (rand() % (max_err + 1)) - (rand() % (max_err + 1));
		switch (flag)
		{	
		case 0:
			if (cmd != 6)
			{
				if (cmd == 1 && est_pos_z < max - max_step)
				{ 
					pos_z += step;
					est_pos_z += step + err;
					if (est_pos_z >= 9975)
					{
						pos_z = 10000;
						est_pos_z = 10000;
					}
					write(fd_z_out, &est_pos_z, sizeof(float));
				}

				if (cmd == 2 && est_pos_z > max_step)
				{ 
					pos_z -= step;
					est_pos_z -= step + err;
					if (est_pos_z <= 25)
					{
						pos_z = 0;
						est_pos_z = 0;
					}
					write(fd_z_out, &est_pos_z, sizeof(float));
				}
			}
			break;
		case 1:
			if (est_pos_z >= max_step)
			{
				est_pos_z -= step + err;

				est_pos_z -= step + err;
				if (est_pos_z <= 25)
				{
					est_pos_z = 0;
				}
			}
			write(fd_z_out, &est_pos_z, sizeof(float));
			if (est_pos_z < max_step)
			{
				flag = 2;
			}
			break;	
		case 2:
			cmd = 6;
			flag = 0;
			break;
		}
		usleep(200000);
	}
	close(fd_z_in);
	close(fd_z_out);
	return 0;
}
