#include <reg51.h>
#include "intrins.h"
typedef unsigned char u8;
typedef unsigned int u16;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

#define GPIO_KEY P1 
#define GPIO_DIG P0 

u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
					0x6f,0x77};
u8 a;			 //记住按键对应的值
void delay(u16 i)
{
	while(i--);
}

void key()	//判断按键位置
{
	u8 b=0;
	  GPIO_KEY=0x0f;
	  if(GPIO_KEY!=0x0f)
	  {
	  	   delay(100);	   //消抖
		   if(GPIO_KEY!=0x0f)
		   {
		   		GPIO_KEY=0x0f;			//保险起见？
				switch(GPIO_KEY)
				{
				  case 0x07:	a=0;	break;	//第一列按键按下（P13口为0）
				  case 0x0b:	a=1;	break;
				  case 0x0d:	a=2;	break;
				  case 0x0e:	a=3;	break;
				}
				GPIO_KEY=0xf0;		   //保险？
				switch(GPIO_KEY)
				{
				  case 0x70:			break;	
				  case 0xb0:	a+=4;	break;
				  case 0xd0:	a+=8;	break;
				  case 0xe0:	a+=12;	break;
				}
				while(GPIO_KEY!=0xf0 && ++b<500)
				{
					delay(100);
				}
		   }
	  }
}
void main()
{
	LSA = 0;
	LSB = 0;
	LSC = 0;
	while(1)
	{		 
		   u8 i;
		   key();
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
				GPIO_DIG = arr[a]; //段选
				P0=0x00;		  //消影
			}		   
	}
}