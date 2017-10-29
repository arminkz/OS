#include <stdio.h>
#include <unistd.h>

int main(){

  printf("Multi Programing (POSIX) : \n");

  printf("Initial PID : %d\n",getpid());
  printf("Initial PPID : %d\n",getppid());

  int pid = fork();
  if(pid<0){
    printf("Fork failed !\n");
  }else if(pid==0){
    printf("Child PID : %d\n",getpid());
    printf("Child PPID : %d\n",getppid());
  }else{
    printf("Parent PID : %d\n",getpid());
    printf("Parent PPID : %d\n",getppid());
  }

  return 0;
}
