/* Application in User Space */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() 
{int dev = open("/dev/mydevice", O_RDONLY);  // open the file driver file created. open(path,mode)
 if(dev == -1)                               // if file does not exist in directory          
 {printf("Opening was not possible!\n");     
  return -1;
 }
 printf("Opening was successfull!\n");
 close(dev);                                  // close the file, if file is not closed it is closed automatically after execution
 
 int dev1 = open("/dev/mydevice", O_RDONLY);  // open the file driver file created. open(path,mode)
 int dev2 = open("/dev/mydevice", O_RDONLY);  // open the file driver file created. open(path,mode)
 int dev3=  open("/dev/mydevice", O_RDONLY);  // open the file driver file created. open(path,mode)
 int dev4 = open("/dev/mydevice", O_RDONLY);  // open the file driver file created. open(path,mode)
 return 0;
}
