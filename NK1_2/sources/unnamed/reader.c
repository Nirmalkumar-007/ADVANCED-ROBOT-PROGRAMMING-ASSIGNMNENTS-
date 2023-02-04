#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#define MSGSIZE 100000

int g1_cycle, g2_cycle;
int sum1=0;
int sum2=0;
int c1 = 0; 
int c2 = 0;

struct message
{
    time_t timestamp;
    char g[50];
    int x;
}
message;

struct msgbuf
{
    struct message buffer[MSGSIZE];
    time_t timestamp[MSGSIZE];
}msgbuf;


int main(int argc, char *argv[])
{
    int fd_r1, fd_r2, fd_r3;
    int x;
    int d1, d2;
    float avg_g1, avg_g2;
    int avg_delay1[MSGSIZE],avg_delay2[MSGSIZE];

    char buf1[20];
    char buf2[20];
    char buf3[20];

    memset(buf1, '\0', sizeof(buf1));
    memset(buf2, '\0', sizeof(buf2));
    memset(buf3, '\0', sizeof(buf3));
    sscanf(argv[1], "%d", &fd_r1);
    sscanf(argv[2], "%d", &fd_r2);
    sscanf(argv[3], "%d", &fd_r3);

    struct message* info_g1;
    struct message* info_g2;
    info_g1 = (struct message*)malloc(sizeof(struct message));
    info_g2 = (struct message*)malloc(sizeof(struct message));


    struct msgbuf* rec_msg1;
    struct msgbuf* rec_msg2;
    rec_msg1 = (struct msgbuf*)malloc(sizeof(struct msgbuf));
    rec_msg2 = (struct msgbuf*)malloc(sizeof(struct msgbuf));

    rec_msg1->buffer[MSGSIZE];
    rec_msg2->buffer[MSGSIZE];

    rec_msg1->timestamp[MSGSIZE];
    rec_msg2->timestamp[MSGSIZE];

    printf("starting the reader");

    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int ret;
    int i=0, j=0; 

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd_r1, &readfds);
        FD_SET(fd_r2, &readfds);
        FD_SET(0, &readfds);
        printf("before select \n");
        ret = select(1, &readfds, NULL, NULL, &tv);
        if (ret <0)
        {
            perror("select");
        }
        else if(ret>0)
        {

                if (FD_ISSET(fd_r1, &readfds))
                {
                    if(read(fd_r1, info_g1, sizeof(*info_g1))<0)
                    {
                        perror("read error");
                        return 1;
                    }
             	    printf("reading from g1");
                    rec_msg1->buffer[i].x=info_g1->x;
                    rec_msg1->buffer[i].x=info_g1->timestamp;
                    time(&rec_msg1->timestamp[i]);
                    strcpy(rec_msg1->buffer[i].g, info_g1->g);
                    avg_delay1[i] = difftime(rec_msg1->timestamp[i], rec_msg1->buffer[i].timestamp);
                    while(i<MSGSIZE)
                    {
                        sum1 += avg_delay1[i];
                        i++;
                    }
                    avg_g1 =sum1/MSGSIZE;
                    usleep(10);
                    read(fd_r1, &d1, sizeof(d1));
                    c1 += d1;
                    d1=c1;
                    usleep(10);
                    read(fd_r1, &g1_cycle, sizeof(g1_cycle));

                }
                if (FD_ISSET(fd_r2, &readfds))
                {
                    if(read(fd_r2, info_g2, sizeof(*info_g2))<0)
                    {
                        perror("read error");
                        return 1;
                    }
             	    printf("reading from g2");
                    rec_msg2->buffer[j].x=info_g2->x;
                    rec_msg2->buffer[j].x=info_g2->timestamp;
                    time(&rec_msg2->timestamp[j]);
                    strcpy(rec_msg2->buffer[j].g, info_g2->g);
                    avg_delay2[j] = difftime(rec_msg2->timestamp[j], rec_msg2->buffer[j].timestamp);
                    while(j<MSGSIZE)
                    {
                        sum2 += avg_delay2[j];
                        j++;
                    }
                    avg_g2 =sum2/MSGSIZE;
                    usleep(10);
                    read(fd_r2, &d2, sizeof(d2));
                    c1 += d2;
                    d2=c1;
                    usleep(10);
                    read(fd_r2, &g2_cycle, sizeof(g2_cycle));
                }

            }
        
            if(g1_cycle ==1000000 && g2_cycle ==1000000)
            {
            
                write(fd_r3,info_g1, sizeof(*info_g1));
                usleep(5);
                write(fd_r3,info_g2, sizeof(*info_g2));
                usleep(5);
                write(fd_r3,&x, sizeof(x));
                usleep(5);
                write(fd_r3,&d1, sizeof(d1));
                usleep(5);
                write(fd_r3,&d2, sizeof(d2));
                usleep(5);
                write(fd_r3,&c1, sizeof(c1));
                usleep(5);
                write(fd_r3,&c2, sizeof(c2));
                usleep(5);
                write(fd_r3,&avg_g1, sizeof(avg_g1));
                usleep(5);
                write(fd_r3,&avg_g2, sizeof(avg_g2));
                usleep(5);
            }
        }

    }
    
    return 0;
}
