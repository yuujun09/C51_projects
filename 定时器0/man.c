#include "reg52.h"
#include "intrins.h"
typedef unsigned char u8;
typedef unsigned int u16;
sbit led=P2^0;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
					0x6f,0x77};
u16 a=0;

void display()
{
	u16 k = 1;
    u8 i;
	u16 b=1000;
	while(b--)
	for(i=0;i<8;i++)
	{
		switch(i)
			{
			case(0):  LSA=LSB=LSC=0; break;
			case(1):  LSA=1;LSB=LSC=0; break;
			case(2):  LSA=0;LSB=1;LSC=0; break;
			case(3):  LSA=LSB=1;LSC=0; break;
			case(4):  LSA=LSB=0;LSC=1; break;
			case(5):  LSA=1;LSB=0;LSC=1; break;
			case(6):  LSA=0;LSB=LSC=1; break;
			case(7):  LSA=LSB=LSC=1; break;				
			}  
		P0=arr[a];

	}	
}

void timer0Init()  //计时器初始化
{
 	TMOD|=0x01;	   //或运算防止拉低电平对之前控制位的影响
	TH0=0XFC;
	TL0=0X18;		//FC18对应一毫秒
	ET0=1;
	EA=1;
	TR0=1;
}

void main()
{
	timer0Init();
	while(1);
}

void time0() interrupt 1 //T0中断号为1
{
	static u16 i;
	TH0=0XFC;		 //方式一需要重新赋初值，方式二能自动重装初值
	TL0=0X18;	
	i++;
	if(i==1000)		   //1ms*1000=1s
	{
	i=0;
	a++;
	display();
	}
}