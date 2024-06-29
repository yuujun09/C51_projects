#include "reg52.h"
#include "intrins.h"
typedef unsigned char u8;
typedef unsigned int u16;
sbit led=P2^0;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77};
u16 a=0;
u16 i=0;

void delay(u16 k)
{
	while(k--);
}

void display(u16 a)
{	 
	u16 k;	
		for(k=0;k<6;k++)
		{
			switch(k)
				{
				case(0):  LSA=LSB=LSC=0;  P0 = arr[a%10]; break;
				case(1):  LSA=1;LSB=LSC=0; P0 = arr[(a/10)%6];  break;
				case(2):  LSA=LSB=1;LSC=0; P0 = arr[(a/60)%10]; break;
				case(3):  LSA=LSB=0;LSC=1; P0 =arr[(a/600)%60%10];  break;
				case(4):  LSA=0;LSB=LSC=1;P0 = arr[(a/3600)%10];  break;
				case(5):  LSA=LSB=LSC=1;  P0 = arr[(a/36000)%3600%10]; break;				
				}  	
			delay(5); 
			P0=0x00;
		}	
}

void timer0Init()  //计时器初始化
{
 	TMOD|=0x01;	   //或运算防止拉低电平对之前控制位的影响
	TH0=0X3C;
	TL0=0XAF;		//对应50毫秒
	ET0=1;
	EA=1;
	TR0=1;
}

void main()
{
	timer0Init();
	while(1)
	{
		if(TF0 == 1)
		{
		TF0 = 0;
		i++;
		TH0=0X3C;
		TL0=0XAF;
		}
		if(i==20)
		{
			a++;
			i=0;
		}
		display(a);
	}
}
