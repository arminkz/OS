#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main(){
   int ret, fd;
   
   printf("Starting device test code example...\n");

   fd = open("/dev/akpchar", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }

   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", receive);

   char stringToSend[BUFFER_LENGTH];
   
   printf("Type in a short string to send to the kernel module:\n");
   scanf("%s", stringToSend);
   printf("Writing message to the device [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   if (ret < 0){
      perror("Kernel module refused the message : ");
      return errno;
   }else{
      printf("Kernel module accepted the message !\n");
   }

   return 0;
}
