#include "reg52.h"
#include "intrins.h"
#include <math.h>
typedef unsigned char u8;
typedef unsigned int u16;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 code arr[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
					0x6f,0x77};

u8 zero_flag = 0;		//清零标志位
u16 time = 0;
u16 temp = 0;
u16 clock = 3600*4+ 60*59 +25;
u8 h1 = 0;
u8 h2 = 0;
u8 m1 = 0;
u8 m2 = 0;
u8 s1 = 0;
u8 s2 = 0;

void delay(u16 i)
{
while(i--);
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

void open0()		//INT0中断初始化
{
	EA=1;	//打开CPU总中断
  EX0=1;	//打开外部中断允许位
	IT0=1;	//下降沿触发		
}

void open1()	//INT1中断初始化
{
	EA=1;	//打开CPU总中断
   EX1=1;	//打开外部中断允许位
	IT1=1;	//下降沿触发		
}

void clock_refresh()		//时钟进制换算
{
	h1 = (clock/36000)%3;
	h2 = (clock/3600)%10;
	m1 = (clock/600)%6;
  m2 = (clock/60)%10;
  s1 = ((clock)/10)%6;	
  s2 = clock%10;
}

void display()
{
  u8 i;
	
	for(i=0;i<6;i++)	//刷新6个位
	{
	switch(i)
		{
		case(0):  LSA=LSB=LSC=0; P0=arr[h1]; break;
		case(1):  LSA=1;LSB=LSC=0; P0=arr[h2]; break;
		case(2):  LSA=0;LSB=1;LSC=0; P0=arr[m1];break;
		case(3):  LSA=LSB=1;LSC=0; P0=arr[m2];break;
		case(4):  LSA=LSB=0;LSC=1; P0=arr[s1]; break;
		case(5):  LSA=1;LSB=0;LSC=1; P0=arr[s2];break;
		//case(6):  LSA=0;LSB=LSC=1; break;
		//case(7):  LSA=LSB=LSC=1; break;				
		}  
	delay(5);
	P0=0x00;
	}
}

void main()
{
	open0();
	open1();
	timer0Init();
	temp = clock;						//保存当前时钟值到temp
	while(1)
	   {
	   display();
	   }
}

void time0() interrupt 1 //T0中断号为1
{
	TH0=0XFC;		 //方式一需要重新赋初值，方式二能自动重装初值
	TL0=0X18;	
	time++;
	if(time==100)		   //1ms*100=0.1s
	{
	time=0;
	clock++;
	clock_refresh();
	}
	display();
}

void zhongduan0() interrupt 0			//启动停止键
{
	P1 = 1;	
	if(TR0==1)	//定时器运行中
	{				
		TR0 = 0;
	}
	else if(TR0==0)	//定时器未运行
	{	
		if(clock != 0 && zero_flag==1)
		{
			clock = temp;
			zero_flag = 0;
		}
		TR0 = 1;
	}
	
	display();
}

void zhongduan1() interrupt 2	  //清零键
{
	temp = clock;						//保存当前时钟值到temp
	zero_flag = 1;			//清零标志位拉高
	P1 = 2;
	clock = 0;						//清零
	TR0=0;
	clock_refresh();
	display();
}
