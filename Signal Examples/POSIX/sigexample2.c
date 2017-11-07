#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Handles Kill Interrupt
void killHandler(int signum){
  printf("[PID] : %d - KILL INTERRUPT \n",getpid());
  return;
}

//Handles Interrupts on Child Termination (Executes on Parent)
void childHandler(int signum){
  printf("[PID] : %d - CHILD INTERRUPT \n",getpid());
  //Get Child Exit Code
  int status;
  wait(&status);
  int wes = WEXITSTATUS(status);
  printf("[WES] : %d \n",wes);
}

int main() {
  printf("[PID] : %d\n",getpid());

  //Register SIGCHLD
  signal(SIGCHLD,childHandler);
  //Register SIGKILL
  if(signal(SIGKILL,killHandler)==SIG_ERR)
  printf("[ERR] : cannot register SIGKILL (signal blocked by OS !)\n");

  int p = fork();
  if(p==0){
    printf("[PID] : %d - EXITING\n",getpid());
    exit(1);
  }else{
    //Send Kill Signal
    //kill(getpid(),SIGKILL);
    wait(NULL);
  }
  return 0;
}
