#include "main.h"
extern "C" {int globalvar;}
int i =0;
int main()
{
while(1){
	    globalvar = i;  //ERROR, globalvar is undefined.
	    i++;
}



}
