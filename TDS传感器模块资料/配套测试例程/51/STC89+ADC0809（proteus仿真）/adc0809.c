#include "adc0809.h"



void Red_0809 ()
{
    
	ST = 0;
	ST = 1;
	ST = 0;
	while(EOC == 0);
	OE = 1;
	num = P3;
	OE = 0;
	
}