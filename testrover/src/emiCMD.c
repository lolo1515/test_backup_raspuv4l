#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"
//#include <string>
#define BUF_SIZE 128
 
int main()
{
  int i=0;
  int cport_nr=24; /* /dev/ttyUSB0 */
  int bdrate=9600; /* 9600 baud */
  char cmd[100]="\0";
  char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  char str_send[2][BUF_SIZE]; // send data buffer
  unsigned char str_recv[BUF_SIZE]; // recv data buffer
  //strcpy(str_send[0], "This is a test string.");
  //strcpy(str_send[1], "This is another test string.");
  FILE* fichier = NULL;
  FILE* fich = NULL;
  char chaine[100] = ""; // Chaîne vide de taille TAILLE_MAX
while(1){
    fichier = fopen("bufferCMD", "r");
 
    if (fichier != NULL)
    {
// while(fichier != NULL){
   
        fgets(chaine, 100, fichier); // On lit maximum TAILLE_MAX caractères du fichier, on stocke le tout dans "chaine"
	printf("emi:%s\n", chaine); // On affiche la chaîne

        if (chaine[0] != '\0'){
	printf("emidifnul:%s", chaine); // On affiche la chaîne

 	if (chaine == "{\"keycodes\":[103]}"){
		strcpy(cmd,"V1");
		printf("cmdV1");
	}
	if (chaine == "{\"keycodes\":[105]}"){
		strcpy(cmd,"V2");
	}
        fclose(fichier);
	printf("fermeture");
	fich = fopen("bufferCMD", "w");
	if(fich!=0) printf("fichier effacer\n");
	fclose(fich); 
	strcpy(chaine,"\0");
	}
    }
if (cmd[0]!='\0'){
printf("cmdvaut:");
printf(cmd);
printf("\n");
//  if (cmd[0]!='\0'){
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");
    return(0);
  }
 
  usleep(2000);  /* waits 2000ms for stable condition */
  
  
    RS232_cputs(cport_nr, cmd); // sends string on serial
    printf("Sent to Arduino: '%s'\n", cmd);
	//usleep(100000);  /* waits for reply 1000ms */
	
	
    usleep(5000);  /* sleep for 5 Second */
//fputs("\0", fich);
strcpy(cmd,"\0");
}
}
}
