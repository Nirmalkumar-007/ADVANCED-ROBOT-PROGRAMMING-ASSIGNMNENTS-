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

#define MSGSIZE 10


int d1,d2;
int g1_cycle,g1_cycle;   
float avg_g1, avg_g2;
long int avg_delay;//avg_delay_sec;
long int bits;
char g[20];

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
    int x;
    int fd3 = open("/tmp/m", O_RDONLY);
    if(fd3 == -1){
        printf("error occured");
        return 1;
    }
    printf("opening m \n");
    
    struct message* message1;
    message1 = (struct message*)malloc(sizeof(struct message));
    struct message* message2;
    message2 = (struct message*)malloc(sizeof(struct message));
    
    struct msgbuf* q1;
    struct msgbuf* q2;
    q1 = (struct msgbuf*)malloc(sizeof(struct msgbuf));
    q2 = (struct msgbuf*)malloc(sizeof(struct msgbuf));


       
    avg_delay = ((avg_g1)+(avg_g2))/2;
    avg_delay_sec=((avg_g1/1000000)+(avg_g2/1000000));
    bits = (sizeof(*message1)*8)+(sizeof(*message2)*8);
    
    
    printf("starting to read \n");
    usleep(3);
    read(fd3, &g1_cycle, sizeof(g1_cycle));
    printf("no of cycles of g1 is %d \n", g1_cycle);
    usleep(3);
    read(fd3, &g1_cycle, sizeof(g1_cycle));
    printf("no of cycles of g2 is %d \n",g1_cycle);
    usleep(3);
    read(fd3, message1, sizeof(*message1));
    printf(" %s no of data recieved is %d \n",message1->g,message1->x);
    usleep(3);
    read(fd3, message2, sizeof(*message2));
    printf(" %s no of data recieved is %d \n",message2->g,message2->x);
    usleep(3);
    read(fd3, &d1, sizeof(d1));
    printf("delay for g1 is %d \n",d1);
    usleep(3);
    read(fd3, &d2, sizeof(d2));
    printf("delay for g2 is %d \n",d2);
    usleep(3);
    read(fd3, &avg_g1, sizeof(avg_g1));
    usleep(3);
    read(fd3, &avg_g2, sizeof(avg_g2));
    usleep(3);
    printf(" average delay between generator and reciever is %ld \n", avg_delay);
    printf(" estimated bandwidth between G and R is %ld", bits%avg_delay_sec);

    return 0;
    

}
