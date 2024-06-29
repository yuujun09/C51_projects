//每10ms使计时器加一，显示0~9999，按S1时开始，s2停止，在停止时s3加一，s4减一
#include "reg52.h"
#include "intrins.h"
#include <math.h>
typedef unsigned char u8;
typedef unsigned int u16;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit k1 = P3^1;
sbit k2 = P3^0;
sbit k3 = P3^2;
sbit k4 = P3^3;


u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
					0x6f,0x77};

u16 i=0;

void delay(u16 x)
{
while(x--);
}

void display()
{
	u16 k;
	for(k=0;k<4;k++)
	{
		switch(k)
		{
			case(0):  LSA=LSB=LSC=0;		P0 = arr[i%10]; 	break;
			case(1):  LSA=1;LSB=LSC=0; 		P0 = arr[i/10%10];		break;
			case(2):  LSA=0;LSB=1;LSC=0; 	P0 = arr[i/100%10];	break;
			case(3):  LSA=LSB=1;LSC=0; 		P0 = arr[i/1000%10];	break;
		}  
		delay(1);
		P0=0x00;
	}
}

void t0_init()
{
	TMOD = 0x01;
	TH0= 0Xdb;
	TL0= 0Xff;
}

void main()
{
	t0_init();
	while(1)
	{
		display();
		if(k1==0)
		{
		 	delay(10);
			if(k1==0)
			{
				TR0 = 1;
				while( !k1 );
			}
		}
		if(k2==0)
		{
		 	delay(10);
			if(k2==0)
			{
				TR0 = 0;
				while( !k2 );
			}
		}	  
		if(k3==0)
		{
		 	delay(10);
			if(k3 == 0 && TR0 == 0)
			{
				i++;
				while( !k3 );
			}
		}
		if(k4==0)
		{
		 	delay(10);
			if(k4==0 && TR0 == 0)
			{		
				i--;
				while( !k4 );
			}
		}
		if(TF0==1)
		{
			TH0= 0Xdb;
			TL0= 0Xff;	
			TF0=0;
			i++;
		}
		if(i==9999)
		{
		i=0;
		}

	}
}