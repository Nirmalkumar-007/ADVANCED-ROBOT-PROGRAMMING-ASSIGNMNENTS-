#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

pid_t pid_insp;

void handler(int sig)
{
    if (sig == SIGUSR1)
    {
        alarm(60);
    }
    if (sig == SIGALRM)
    {
        
        kill(pid_insp, SIGUSR1);
        alarm(60);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    int fd_insp;
    fd_insp = open(argv[1], O_RDONLY);
    read(fd_insp, &pid_insp, sizeof(int));
    while (1)
    {
        sleep(1);
    }
    close(fd_insp);
    return 0;
}
