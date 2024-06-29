#include <reg51.h>
#include "intrins.h"
 
sbit led = P2^0;	  //默认高电平
sbit k3 = P3^2;
sbit k4 = P3^3;
sbit SCK=P3^6;
sbit RCK=P3^5;
sbit SER=P3^4;

typedef unsigned char u8;
typedef unsigned int u16;

u8 col[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xde};
//位选（第几列亮）  :第一列到第八列		 			  

u8 row[]={0x00,0x00,0x3e,0x41,0x41,0x41,0x3e,0x00};
//段选（第几行亮）			 0010 0001
 

void delay(u16 i)
{
	while(i--);
}

void open0()
{
	EA=1;	//打开CPU总中断
    EX0=1;	//打开外部中断允许位
	IT0=1;	//下降沿触发		
}

void open1()
{
	EA=1;	//打开CPU总中断
    EX1=1;	//打开外部中断允许位
	IT1=1;	//下降沿触发		
}

void dz(u8 dat)	
{
	u8 a;
	SCK=0;
	RCK=0;	//拉低电平
	for(a=0;a<8;a++) //将数据逐位输入到寄存器内
	{
	SER=dat>>7;
	dat<<=1;
	SCK=1;
	_nop_();//延时一个机器周期
	_nop_();
	SCK=0;   //拉低电平，为下一次上升沿做准备
	}
	RCK=1;	  //
	_nop_();
	_nop_();
	RCK=0;
}

void main()
{
	open0();
	open1();
	while(1);		 	  
}

void zhongduan0() interrupt 0
{
	delay(100);   //按键消抖
	if(k3==0)
	{
		led=~led;
	}	
}

void zhongduan1() interrupt 2	  //外部中断1的中断号为2！
{
	delay(100);   //按键消抖
	if(k4==0)
	{
		u8 i,j;
		P0=0x7f;
		for(j=0;j<100;j++)	
			for(i=0;i<8;i++)
			{
			P0=col[i];
			dz(row[i]);
			delay(100);
			dz(0x00);					 		
			}
	}	
}