#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"
 
#define BUF_SIZE 128
 
int main()
{
  int i=0;
  int cport_nr=16; /* /dev/ttyUSB0 */
  int bdrate=9600; /* 9600 baud */
  int error = 0;
  char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  char str_send[2][BUF_SIZE]; // send data buffer
  unsigned char str_recv[BUF_SIZE]; // recv data buffer
  strcpy(str_send[0], "T$");
  //strcpy(str_send[1], "This is another test string.");
  
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");
    return(0);
  }
 
  usleep(2000000);  /* waits 2000ms for stable condition */
  while(1)
  {
    RS232_cputs(cport_nr, str_send[0]); // sends string on serial
//	printf("Sent to Arduino: '%s'\n", str_send[0]);
	//usleep(100000);  /* waits for reply 1000ms */
	int n = RS232_PollComport(cport_nr, str_recv, (int)BUF_SIZE);
	if(n > 0){
	if ((n >= 60)&&(n<=70)) {
      str_recv[n] = 0;   /* always put a "null" at the end of a string! */
  //    printf("Received %i bytes: '%s'\n", n, (char *)str_recv);
	}
	else {
		printf("failrecp");
		error = 1;
	}
	}
	i++;
	if (i == 5){
	i == 0;
	usleep(2000000);    
	}
    usleep(350000);  /* sleep for 5 Second */
	if (error == 0){
  
    FILE* fichier = NULL;
 
    fichier = fopen("bufferGPS", "w+");
 
    if (fichier != NULL)
    {
        fputs((char *)str_recv, fichier);
        fclose(fichier);
    }
 }
error = 0;
  }
}
