#include <reg51.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int 
sbit led = P2^0;
sbit k1 = P3^1;
typedef unsigned char u8;
typedef unsigned int u16;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 keysit = 1;

u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,
				0x6d,0x7d,0x07,0x7f,0x6f,0x77};

int main()
{
	u8 i = 0;
	u8 backup = 1;
	EA = 1;
	TMOD = 0X01;
	TH0 = 0XF8;
	TL0 = 0XCD;
	ET0 = 1;
	TR0 = 1;
	LSA = LSB = LSC = 0;
	P0 = arr[i];


	while(1)
	{
		if(backup != keysit)   //0    1
		{
		 	if(backup == 0)
			{
				i++;
				if(i==10) i = 0;
				P0 = arr[i];
			}
		backup = keysit;		 //0    0
		}
	}
}

void InterruptTimer0()	interrupt 1
{
	static u8 keybuf = 0xFF; 
	TH0 = 0XF8;
	TL0 = 0XCD;

	keybuf = (keybuf << 1) | k1;
	if(keybuf == 0xff)
	{
		keysit = 1;
	}	
	else if(keybuf == 0x00)	//0000 0000
	{
		keysit = 0;
	}
}