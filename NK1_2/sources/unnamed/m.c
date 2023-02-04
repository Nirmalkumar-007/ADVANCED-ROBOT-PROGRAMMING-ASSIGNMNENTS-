#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <time.h>


#define MSGSIZE 1000000

int d1,d2,g1_cycle,g1_cycle;   
int avg_g1, avg_g2;

struct message{
    int x;
    char g[20];
    time_t timestamp;
}
message;

struct msgbuf{
    struct message buffer[MSGSIZE];
    time_t timestamp[MSGSIZE];
}
msgbuf;

int main(int argc, char* argv[])
{
    char g[20];
    int x;
    int fd_m;
    char buf1[20];
    memset(buf1, '\0', sizeof(buf1));
    sscanf(argv[1], "%d", &fd_m);
    struct message* info_g1;
    struct message* info_g2;
    info_g1 = (struct message*)malloc(sizeof(struct message));
    info_g2 = (struct message*)malloc(sizeof(struct message));
    
    struct msgbuf* rec_msg1;
    struct msgbuf* rec_msg2;
    rec_msg1 = (struct msgbuf*)malloc(sizeof(struct msgbuf));
    rec_msg2 = (struct msgbuf*)malloc(sizeof(struct msgbuf));

    while(1)
    {   
        int avg_delay = ((avg_g1)+(avg_g2))/2;
        int avg_delay_sec=((avg_g1/1000000)+(avg_g2/1000000));
        int bits = (sizeof(*info_g1)*8)+(sizeof(*info_g2)*8);
        int bits1 = bits%avg_delay_sec;
        info_g1->g;
        info_g1->x;
        info_g2->g;
        info_g2->x;
        usleep(3);
        read(fd_m, &g1_cycle, sizeof(g1_cycle));
        printf("no of cycles of g1 is %d \n", g1_cycle);
        usleep(3);
        read(fd_m, &g1_cycle, sizeof(g1_cycle));
        printf("no of cycles of g2 is %d \n",g1_cycle);
        usleep(3);
        read(fd_m, info_g1, sizeof(*info_g1));
        printf(" %s no of data recieved is %d \n",g,x);
        usleep(3);
        read(fd_m, info_g2, sizeof(*info_g2));
        printf(" %s no of data recieved is %d \n",g,x);
        usleep(3);
        read(fd_m, &d1, sizeof(d1));
        printf("delay for g1 is %d \n",d1);
        usleep(3);
        read(fd_m, &d2, sizeof(d2));
        printf("delay for g2 is %d \n",d2);
        usleep(3);
        read(fd_m, &avg_g1, sizeof(avg_g1));
        usleep(3);
        read(fd_m, &avg_g2, sizeof(avg_g2));
        usleep(3);
        printf(" average delay between generator and reciever is %d \n", avg_delay);
        printf(" estimated bandwidth between G and R is %d", bits1);

        return 0;
    }

}
