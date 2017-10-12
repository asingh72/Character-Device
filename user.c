
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 25600              
static char receive[BUFFER_LENGTH];    

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   
   fd = open("/dev/process_list", O_RDWR);            
   if (fd < 0){
      printf("Failed to open ");
      return errno;
   }
   
   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        
   if (ret < 0){
      printf("Failed to read");
      return errno;
   }
   printf("%s\n", receive);
  
   return 0;
}
