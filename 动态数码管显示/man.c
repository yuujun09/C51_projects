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

u8 zero_flag = 0;		//�����־λ
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

void timer0Init()  //��ʱ����ʼ��
{
 	TMOD|=0x01;	   //�������ֹ���͵�ƽ��֮ǰ����λ��Ӱ��
	TH0=0XFC;
	TL0=0X18;		//FC18��Ӧһ����
	ET0=1;
	EA=1;
	TR0=1;
}

void open0()		//INT0�жϳ�ʼ��
{
	EA=1;	//��CPU���ж�
  EX0=1;	//���ⲿ�ж�����λ
	IT0=1;	//�½��ش���		
}

void open1()	//INT1�жϳ�ʼ��
{
	EA=1;	//��CPU���ж�
   EX1=1;	//���ⲿ�ж�����λ
	IT1=1;	//�½��ش���		
}

void clock_refresh()		//ʱ�ӽ��ƻ���
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
	
	for(i=0;i<6;i++)	//ˢ��6��λ
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
	temp = clock;						//���浱ǰʱ��ֵ��temp
	while(1)
	   {
	   display();
	   }
}

void time0() interrupt 1 //T0�жϺ�Ϊ1
{
	TH0=0XFC;		 //��ʽһ��Ҫ���¸���ֵ����ʽ�����Զ���װ��ֵ
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

void zhongduan0() interrupt 0			//����ֹͣ��
{
	P1 = 1;	
	if(TR0==1)	//��ʱ��������
	{				
		TR0 = 0;
	}
	else if(TR0==0)	//��ʱ��δ����
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

void zhongduan1() interrupt 2	  //�����
{
	temp = clock;						//���浱ǰʱ��ֵ��temp
	zero_flag = 1;			//�����־λ����
	P1 = 2;
	clock = 0;						//����
	TR0=0;
	clock_refresh();
	display();
}
