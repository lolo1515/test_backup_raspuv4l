#include <sstream>
#include <cstdint>
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"
#include <string>
#include <iostream>
    #include <cstring>
#define BUF_SIZE 128

int main()
{
    int i=0;
    int cport_nr=16; /* /dev/ttyUSB0 */
    int bdrate=57600; /* 9600 baud */

    char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
    char str_send[2][BUF_SIZE]; // send data buffer
    unsigned char str_recv[BUF_SIZE]; // recv data buffer
    strcpy(str_send[0], "epi");
    strcpy(str_send[1], "puc");
   // strcpy(str_send[2], "q");
    //strcpy(str_send[3], "q");

    if(RS232_OpenComport(cport_nr, bdrate, mode))
    {
        printf("Can not open comport\n");
        return(0);
    }

    usleep(2000000);  /* waits 2000ms for stable condition */
    while(1)
    {
        RS232_cputs(cport_nr, str_send[i]); // sends string on serial
        printf("Sent to Arduino: '%s'\n", str_send[i]);
        usleep(1000000);  /* waits for reply 1000ms */
 	std::string testS;
	std::stringstream testStream;
        int n = RS232_PollComport(cport_nr, str_recv, (int)BUF_SIZE);
        if(n > 0){
            str_recv[n] = 0;   /* always put a "null" at the end of a string! */
            printf("Received %i bytes: '%s'\n", n, (char *)str_recv);
	   // testS.append((const char*)str_recv);       	   
 //testS.assign(str_recv,strlen(str_recv));
//		testS = std::string(str_recv);
		for(int i=0 ; str_recv[i] != '\0' ; ++i)
                    {
                        // std::cout << data_[i];
                        /*datarecu << data_[i];
                        datarecu >> datacast;
                        std::cout << datarecu << std::endl;*/
                        testS += std::string(1,str_recv[i]);
                        //      std::cout << recu << std::endl;
                    }

	//	testS = (char *)str_recv;
	printf("Received string %i bytes: '%s'\n", n, testS);
	}
		
        i++;
        i %= 2;
        usleep(5000000);  /* sleep for 5 Second */

    }
    return(0);
}
