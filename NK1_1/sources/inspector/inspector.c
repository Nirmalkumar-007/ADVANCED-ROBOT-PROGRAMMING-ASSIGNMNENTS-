#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <math.h>
typedef struct
{
    int x;
    int z;
} Position;
char buttons;
int flag = 0;
int back_to_start = 1;
pid_t my_pid, pid_mx, pid_mz, pid_cmd, pid_wd;

void handler(int sig)
{
    if (sig == SIGUSR1)
    { //watchdog
        flag = 1;
    }
    if (sig == SIGINT)
    {

        kill(getppid(), SIGTERM);
        kill(getpid(), SIGTERM);
    }
}

int main(int argc, char *argv[])
{
    fd_set rset;
    struct timeval tv = {0, 0};
    struct sigaction sa;
    sa.sa_handler = &handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    Position position;
    int retval, fd_x, fd_z, fd_cmd, fd_wd, max, max_step, step, max_err;
    step = 20;
    max_err = 4;
    max_step = step + max_err;
    my_pid = getpid();
    pid_wd = atoi(argv[1]);
    max = 10000;
    if((fd_x = open(argv[2], O_RDONLY)) < 0){
        perror("Error opening pos x ");
        return 1;
    }
    if((fd_z = open(argv[3], O_RDONLY)) < 0){
        perror("Error opening pos z ");
        return 1;
    }
    if((fd_cmd = open(argv[4], O_RDONLY)) < 0){
        perror("Error opening pos z ");
        return 1;
    }
    if((fd_wd = open(argv[5], O_WRONLY)) < 0){
        perror("Error opening cmd x ");
        return 1;
    };
    write(fd_wd, &my_pid, sizeof(int));
    read(fd_x, &pid_mx, sizeof(int));
    read(fd_z, &pid_mz, sizeof(int));
    read(fd_cmd, &pid_cmd, sizeof(int));
    printf("S: emergency stop, R: reset\n\n");
    while (1)
    {
        
        if (flag == 1)
        { 

            if(kill(pid_mx, SIGUSR1));
            if(kill(pid_mz, SIGUSR1));
            if(kill(pid_cmd, SIGUSR1));
            back_to_start = 0;
            flag = 0;
        }
        FD_ZERO(&rset);
        FD_SET(fd_x, &rset);
        FD_SET(fd_z, &rset);
        FD_SET(0, &rset);
        retval = select(FD_SETSIZE, &rset, NULL, NULL, &tv);
        fflush(stdout);
        if (retval == -1)
        {
            perror("select()");
        }
        else if (retval >= 1)
        {
            if (FD_ISSET(fd_x, &rset) > 0)
            {
                if(read(fd_x, &position.x, sizeof(float)));
            }
            if (FD_ISSET(fd_z, &rset) > 0)
            {
                if(read(fd_z, &position.z, sizeof(float)));
            }
            if (FD_ISSET(0, &rset) > 0)
            {
                if(read(0, &buttons, sizeof(char)));

                switch (buttons)
                {    
                case 'r':
                    printf("\nRESET\n");
                    if(kill(pid_wd, SIGUSR1));      
                    if(kill(pid_mx, SIGUSR1)); 
                    if(kill(pid_mz, SIGUSR1));
                    if(kill(pid_cmd, SIGUSR1));
                    back_to_start = 0;
                    break;
                    
                case 's':
                    printf("\nSTOP\n");
                    if(kill(pid_wd, SIGUSR1));
                    if(kill(pid_mx, SIGUSR2));
                    if(kill(pid_mz, SIGUSR2));
                    if(kill(pid_cmd, SIGUSR2));
                    break;
                    
                }
            }
        }
        printf("\rposition= (%d mm,%d mm)", position.x, position.z);
        usleep(200000);
        if (position.x <= max_step && position.z <= max_step && back_to_start == 0)
        {

            if(kill(pid_cmd, SIGUSR2));
            back_to_start = 1;
        }
    }
    if(close(fd_cmd));
    if(close(fd_x));
    if(close(fd_z));
    if(close(fd_wd));
    return 0;
}
