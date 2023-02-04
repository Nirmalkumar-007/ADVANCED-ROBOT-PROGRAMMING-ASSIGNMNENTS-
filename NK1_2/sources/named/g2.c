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



int fd2;
int c2=0;
struct message
{
    time_t timestamp;
    char g[2];
    int x;
}
message;


int main(int argc, char *argv[])
{
    
    int g2_cycle;
    if (mkfifo("/tmp/g2", 0666) == -1){
        if (errno != EEXIST){
            printf(" fifo file cannot be created.\n");
            return 1;
        }
    }
    printf("opening g2\n");
    struct message* g2;
    g2 =(struct message *)malloc(sizeof(struct message));
    struct message* message2;
    message2=(struct message *)malloc(sizeof(struct message));
    g2->x = 5;
    g2->timestamp;

    strcpy(g2->g,"g2");
    fd2 = open("/tmp/g2", O_WRONLY);
    if(fd2 == -1){
	printf("error occured");
        return 1;
    }
    printf("ready to write");
    for (g2_cycle=0; g2_cycle <= 10; g2_cycle++){
        int min=0;
        int max =15;
        srand(time(0));
        int d2 =(rand() % (max - min +1)) + min;
        usleep(10*d2);
        write(fd2, &g2, sizeof(*g2));
        time(&g2->timestamp);
	c2++;
        g2->x = c2;
    }
    close(fd2);
    return 0;
}
