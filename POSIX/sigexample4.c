#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;
//Handles Interrupts on Child Termination (Executes on Parent)
void childHandler(int signum){
  printf("[PID] : %d - CHILD INTERRUPT \n",getpid());
  //Get Child Exit Code
  int status;
  wait(&status);
  int wes = WEXITSTATUS(status);
  printf("[WES] : %d \n",wes);
  count += wes;
}

int main() {
  printf("[PID] : %d\n",getpid());

  //Register SIGCHLD
  signal(SIGCHLD,childHandler);

  int i = 0;
  pid_t pid[5];
  for(i=0;i<5;i++){
    pid[i] = fork();
    if(!pid[i]){
      printf("[PID] : %d - EXITING...\n",getpid());
      exit(i);
    }
  }

  //Parent Only

  //Kill all Childs
  for(i=0;i<5;i++){
    kill(pid[i],SIGKILL);
  }

  //Print Childs
  /*printf("My Childs : \n");
  for(i=0;i<5;i++){
    printf("%d ",pid[i]);
  }
  printf("\n");*/

  sleep(5);
  printf("COUT : %d\n",count);

  return 0;
}
