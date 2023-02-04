#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int fd1;
int c1=0;

struct message
{
    time_t timestamp;
    char g[30];
    int x;
}
message;


int main(int argc, char *argv[])
{
    
    int g1_cycle;
    if (mkfifo("/tmp/g1", 0666) == -1){
        if (errno != EEXIST){
            printf(" fifo file cannot be created.\n");
            return 1;
        }
    }
    printf("opening g1\n");
    struct message* g1;
    g1 =(struct message *)malloc(sizeof(struct message));
    struct message* message1;
    message1=(struct message *)malloc(sizeof(struct message));
    g1->x = 5;
    g1->timestamp;

    strcpy(g1->g,"g1");
    fd1 = open("/tmp/g1", O_WRONLY);
    printf("opened \n");
    if(fd1 == -1){
        printf("error occured");
	return 1;
    }
    printf("ready to write");
    for (g1_cycle=0; g1_cycle <= 10; g1_cycle++){
        int min=0;
        int max =15;
        srand(time(0));
        int d1 = (rand() % (max - min +1)) + min;
        usleep(10*d1);
        write(fd1, &g1, sizeof(*g1));
        time(&g1->timestamp);
        c1++;
        g1->x = c1;
    }
    close(fd1);
    return 0;
}
