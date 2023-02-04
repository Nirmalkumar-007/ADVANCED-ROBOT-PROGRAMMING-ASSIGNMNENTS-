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

int c1, c2;
int count1=0, count2=0;
struct message
{   
    time_t timestamp;
    char g[30];
    int x;
}
message;


int main(int argc, char *argv[])
{
    int fd_wr1[2];
    int fd_wr2[2];
    int fd_m[2];

    char buf1[20];
    char buf2[20];
    char buf3[20];
    char buf4[20];

    struct message* g1;
    struct message* g2;
    struct message* info_g1;
    struct message* info_g2;

    g1 = (struct message*)malloc(sizeof(struct message));
    g2 = (struct message*)malloc(sizeof(struct message));
    info_g1 = (struct message*)malloc(sizeof(struct message));
    info_g2 = (struct message*)malloc(sizeof(struct message));

    g1->x=1;
    g2->x=1;

    g1->timestamp;
    g2->timestamp;

    strcpy(g1->g,"g1");
    strcpy(g2->g,"g2");


    int pid1 = fork();
    int pid2 = fork();
    int pid3 = fork();
    if (pipe(fd_wr1) == -1 && pipe(fd_wr2) == -1 && pipe(fd_m) == -1)
    {
        printf("an error occured with open pipe");
        return 1;
    }
    else if(pid1 !=0 && pid2 !=0 && pid3 !=0)
    {
        if (pid1 !=0)
        {
            printf("pipe for g1 created successfully ");
            sprintf(buf1,"%d",fd_wr1[0]);
            int g1_cycle;
            for (g1_cycle=0; g1_cycle <= 1000000; g1_cycle++)
            {
                int min=0;
                int max =20;
                srand(time(NULL));
                int random = (rand() % (max - min +1)) + min;
                int d1 =max+random;
                usleep(10*d1);
                write(fd_wr1[1],g1, sizeof(g1));
                time(&g1->timestamp);
                count1+=1;
                g1->x = count1;
                usleep(2);
                write(fd_wr1[1],&d1, sizeof(d1));
                usleep(2);
                write(fd_wr1[1],&c1, sizeof(c1));
            }   

        }

        if(pid2 !=0)
        {
            printf("pipe for g2 created successfully ");
            sprintf(buf2, "%d", fd_wr2[0]);
            int g2_cycle;
            for (g2_cycle=0; g2_cycle <= 1000000; g2_cycle++)
            {
                int min=0;
                int max =20;
                srand(time(NULL));
                int random = (rand() % (max - min +1)) + min;
                int d2 =max+random;
                usleep(10*d2);
                write(fd_wr2[1],g2, sizeof(g2));
                time(&g2->timestamp);
                count2 +=1;
                g2->x = count1;
                usleep(2);
                write(fd_wr2[1],&d2, sizeof(d2));
                usleep(2);
                write(fd_wr2[1],&c2, sizeof(c2));
            }       
        }

        if(pid3 != 0)
        {
            sprintf(buf3, "%d", fd_m[0]);
            sprintf(buf4, "%d", fd_m[0]);
            execl("./reader","writer",buf1,buf2,buf3,NULL,NULL,NULL);
            execl("./m","writer",buf4,NULL);

        }
        
    }
    return 0;
}
