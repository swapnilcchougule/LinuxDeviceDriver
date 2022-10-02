/* Application in User Space */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define Buf_Size 1024

int8_t write_buf[Buf_Size];     // write buffer  
//int8_t read_buf[Buf_Size];    // read buffer

int main()
{int fd;       // variable file descriptor (File descriptor is integer that uniquely identifies an open file of the process.)
 char option;  // variable for selecting option
 printf("Welcome to character device driver Application. \n" );
 
 fd = open("/dev/pcd",O_RDWR); // open the file driver file created. open(path,mode)
                               // O_RDWR: read and write. (O_RDONLY,O_WRONLY,O_CREAT,O_EXCL)
 if(fd < 0)                    // if file does not exist in directory 
 {printf("Cannot open device file \n"); // print 
  return 0;
 }
 
 while(1)
 {printf("**** Please enter the option ****\n");  // print the following lines as the application starts
  printf("           1.Write               \n");  
  printf("           2.Read                \n");
  printf("           3.Exit                \n");
  scanf("%c",&option);
  printf("Your option = %c\n", option);
  
 switch(option)
	{
	case '1':
		printf("Enter the string to write into the driver:\n"); 
		scanf(" %[^\t\n]s",write_buf);
		printf("Date written\n");
		write(fd,write_buf,strlen(write_buf)+1);
		printf("DONE....\n");
		sleep(1);
		break;
	
	case '2':
		printf("Data is reading....\n");
		read(fd,write_buf,1024);
		printf("done....\n\n");
		printf("Data = %s \n\n", write_buf);
		sleep(1);
		break;
	
	case '3':
		close(fd);
		exit(1);
        	break;

	default:
		printf("Enter valid option = %c\n",option);	
		break;			 
  } 
  close(fd);
 }
}

























