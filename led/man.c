#include "reg52.h"
#include "intrins.h"
#include <math.h>
typedef unsigned char u8;
typedef unsigned int u16;
#define led P2
void delay(int i)
{
while(i--);
}
void main()
{
	while(1)
	{
		u8 i = 0;
		u8 n = 0;
		led = 0xfe;	   //1111  1110
		for(i=1;i<8;i++)		  //1111  1100								
		{
			 led -=  pow(2,i);
			 delay(5000);
		}	
					   //0000  0000
		for(i=0;i<8;i++)
		{
			led += pow(2,i);		//0000 0001
			delay(5000);
		}	
		for(i=0;i<8;i++)
		{
			led -= pow(2,7-i);		//0111 1111
			delay(5000);
		}
									//0000 0000
		for(i=0;i<8;i++)
		{
			led += pow(2,8-i);		//1000 0000
			delay(5000);
		}						
	}
}