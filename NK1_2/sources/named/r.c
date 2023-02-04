#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <string.h>

#define MSGSIZE 10



int g1_cycle, g2_cycle, sum1, sum2, d1, d2;
float avg_g1, avg_g2;
int c1 = 0; 
int c2 = 0;
int avg_delay1[MSGSIZE],avg_delay2[MSGSIZE];

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

int main(char *argv[])
{
    int x;
    if (mkfifo("/tmp/m", 0666) == -1){
        if (errno != EEXIST){
            printf(" fifo file cannot be created.\n");
            return 1;
        }
    }
    int fd1 = open("/tmp/g1", O_RDONLY);
    if(fd1 == -1){
	printf("error occured");
        return 1;
    }
    int fd2 = open("/tmp/g2", O_RDONLY);
    if(fd2 == -1){
	printf("error occured");
        return 1;
    }
    
    int fd3 = open("/tmp/m", O_WRONLY);
    if(fd3 == -1)
    {
	printf("error occured");
        return 1;
    }
    

    struct message* message1;
    message1 = (struct message*)malloc(sizeof(struct message));
    struct message* message2;
    message2 = (struct message*)malloc(sizeof(struct message));

    struct msgbuf* rec_msg1;
    struct msgbuf* rec_msg2;
    rec_msg1 = (struct msgbuf*)malloc(sizeof(struct msgbuf));
    rec_msg2 = (struct msgbuf*)malloc(sizeof(struct msgbuf));

    rec_msg1->buffer[MSGSIZE];
    rec_msg2->buffer[MSGSIZE];

    rec_msg1->timestamp[MSGSIZE];
    rec_msg2->timestamp[MSGSIZE];

    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int ret;
    int i=0, j=0; 

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);
        FD_SET(0, &readfds);
        
        ret = select(FD_SETSIZE + 1, &readfds, NULL, NULL, &tv);
        if (ret <0){
            perror("select");
        }
        else if(ret>0)
        {
                if (FD_ISSET(fd1, &readfds))
                {
                    if(read(fd1, message1, sizeof(*message1))<0)
                    {
                        perror("read error");
                        return 1;
                    }
		    printf("reading from g1 \n");
                    rec_msg1->buffer[i].x=message1->x;
                    rec_msg1->buffer[i].timestamp=message1->timestamp;
                    time(&rec_msg1->timestamp[i]);
                    strcpy(rec_msg1->buffer[i].g, message1->g);
                    avg_delay1[i] = difftime(rec_msg1->timestamp[i], rec_msg1->buffer[i].timestamp);
                    avg_g1 =sum1/MSGSIZE;
                    c1 += d1;
                    d1=c1;

                }
                if (FD_ISSET(fd2, &readfds))
                {
		            if(read(fd2, message2, sizeof(*message2))<0)
                    {
                        perror("read error");
                        return 1;
                    }		
		    printf("reading from g2 \n");
                    rec_msg2->  buffer[j].x=message2->x;
                    rec_msg2->buffer[j].x=message2->timestamp;
                    time(&rec_msg2->timestamp[j]);
                    strcpy(rec_msg2->buffer[j].g, message2->g);
                    avg_delay2[j] = difftime(rec_msg2->timestamp[j], rec_msg2->buffer[j].timestamp);
                    avg_g2 =sum2/MSGSIZE;
                    c2 += d2;
                    d2=c2;
                }

        
                if(g1_cycle ==10 && g2_cycle ==10)
                {
                    printf("writing to m");
                    write(fd3,message1, sizeof(*message1));
                    usleep(2);
                    write(fd3,message2, sizeof(*message2));
                    usleep(2);
                    write(fd3,&x, sizeof(x));
                    usleep(2);
                    write(fd3,&d1, sizeof(d1));
                    usleep(2);
                    write(fd3,&d2, sizeof(d2));
                    usleep(2);
                    write(fd3,&g1_cycle, sizeof(g1_cycle));
                    usleep(2);
                    write(fd3,&g2_cycle, sizeof(g2_cycle));
                    usleep(2);
                    write(fd3,&avg_g1, sizeof(avg_g1));
                    usleep(2);
                    write(fd3,&avg_g2, sizeof(avg_g2));
                    usleep(2);
                }

        }
    }
        close(fd1);
        close(fd2);
        close(fd3);
        return 0;
}

