#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int spawn(const char *program, char **arg_list)
{

  pid_t child_pid = fork();
  if (child_pid != 0)
    return child_pid;

  else
  {
    execvp(program, arg_list);
    
    perror("exec failed");
    return 1;
  }
}
void create_pipe(const char *name)
{
  
  if (mkfifo(name, 0666) == -1)
  {
    if (errno != EEXIST)
    {
      perror("Error creating named fifo\n");
      exit(1);
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf(stderr, "usage:%s <filename>\n", argv[0]);
    exit(-1);
  }
  pid_t pid_cmd_k, pid_mx, pid_mz, pid_insp_k, pid_wd;
  char pid_wd_str[20];
  char *cmd_x = "/tmp/fifo_cmd_x";
  char *cmd_z = "/tmp/fifo_cmd_z";
  char *posmotor_x = "/tmp/fifo_posmotor_x";
  char *posmotor_z = "/tmp/fifo_posmotor_z";
  char *cmd_insp = "/tmp/fifo_cmd_insp";
  char *insp_wd = "/tmp/fifo_insp_wd";

  printf("Master PID [%d]\n", getpid());
  fflush(stdout);
  
  char *arg_list_5[] = {"./watchdog", insp_wd, NULL};
  pid_wd = spawn("./watchdog", arg_list_5);

  printf("WatchDog PID [%d]\n", pid_wd);
  fflush(stdout);
  sprintf(pid_wd_str, "%d", pid_wd);
  char *arg_list_1[] = {"./motorx", cmd_x, posmotor_x, NULL};
  char *arg_list_2[] = {"./motorz", cmd_z, posmotor_z, NULL};
  char *arg_list_3[] = {"/usr/bin/konsole", "-e", "./command", pid_wd_str, cmd_x, cmd_z, cmd_insp, (char *)NULL};
  char *arg_list_4[] = {"/usr/bin/konsole", "-e", "./inspection", pid_wd_str, posmotor_x, posmotor_z, cmd_insp, insp_wd, (char *)NULL};

  create_pipe(cmd_x);
  create_pipe(cmd_z);
  create_pipe(posmotor_x);
  create_pipe(posmotor_z);
  create_pipe(cmd_insp);
  create_pipe(insp_wd);
  
  pid_mx = spawn("./motorx", arg_list_1);
  printf("Motor x PID [%d]\n", pid_mx);
  fflush(stdout);
  pid_mz = spawn("./motorz", arg_list_2);
  printf("Motor z PID [%d]\n", pid_mz);
  fflush(stdout);
  pid_cmd_k = spawn("/usr/bin/konsole", arg_list_3);
  printf("Command Konsole PID [%d]\n", pid_cmd_k);
  fflush(stdout);
  pid_insp_k = spawn("/usr/bin/konsole", arg_list_4);
  printf("Inspector Konsole PID [%d]\n", pid_insp_k);
  fflush(stdout);
  int status;
  int first = wait(&status);
  printf("The process with PID [%d] is the First Process terminated with exit status: %d\n", first, status);
  fflush(stdout);

  unlink(cmd_x);
  if(mkfifo(cmd_x, 0666)==-1) {
    perror("error creating pipe for com_x");
    return 1;
  }
  
  unlink(cmd_z);
  if(mkfifo(cmd_z, 0666) == -1){
    perror("error creating pipe for com_z");
    return 1;
  }
  
  unlink(posmotor_x);
  if(mkfifo(posmotor_x, 0666) == -1){
    perror("error creating pipe for pos_x");
    return 1;
  }
  
  unlink(posmotor_z);
  if(mkfifo(posmotor_z, 0666) == -1){
    perror("error creating pipe for pos_z");
    return 1;
  }  

  unlink(cmd_insp);
  if(mkfifo(cmd_insp, 0666) == -1){
    perror("error creating pipe for pos_z");
    return 1;
  }

  unlink(insp_wd);
  if (mkfifo(insp_wd, 0666) == -1){
    perror("error creating pipe for pos_z");
    return 1;
  }
  int n =0;
  
  if (first == pid_insp_k){
    if(kill(pid_cmd_k, SIGTERM)== -1 && errno != ESRCH){
        printf("The Command process has been killed returning: %d\n", n);
        fflush(stdout);
    }
    if(kill(pid_insp_k, SIGTERM) == -1 && errno != ESRCH){
        printf("The inspector process has been killed returning: %d\n", n);
        fflush(stdout);
    }

    if(kill(pid_mx, SIGTERM) == -1 && errno != ESRCH){
        printf("The Motor X process has been killed returning: %d\n", n);
        fflush(stdout);
    }
    if(kill(pid_mz, SIGTERM) == -1 && errno != ESRCH){
        printf("The Motor Z process has been killed returning: %d\n", n);
        fflush(stdout);
    }
    if(kill(pid_wd, SIGTERM) == -1 && errno != ESRCH){
        printf("The WatchDog process has been killed returning: %d\n", n);
        fflush(stdout);
    }
  }
  printf("End of the master process\n");
  fflush(stdout);
  return 0;
}
