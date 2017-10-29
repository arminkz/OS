#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sighandler(int signum){
  printf("interrupt got !\n");
  exit(1);
}

int main() {

  signal(SIGINT,sighandler);

  while(1){
    printf("waiting... \n");
    sleep(1);
  }

  return 0;
}
