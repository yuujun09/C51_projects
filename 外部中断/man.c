#include <reg51.h>
#include "intrins.h"
 
sbit led = P2^0;	  //Ĭ�ϸߵ�ƽ
sbit k3 = P3^2;
sbit k4 = P3^3;
sbit SCK=P3^6;
sbit RCK=P3^5;
sbit SER=P3^4;

typedef unsigned char u8;
typedef unsigned int u16;

u8 col[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xde};
//λѡ���ڼ�������  :��һ�е��ڰ���		 			  

u8 row[]={0x00,0x00,0x3e,0x41,0x41,0x41,0x3e,0x00};
//��ѡ���ڼ�������			 0010 0001
 

void delay(u16 i)
{
	while(i--);
}

void open0()
{
	EA=1;	//��CPU���ж�
    EX0=1;	//���ⲿ�ж�����λ
	IT0=1;	//�½��ش���		
}

void open1()
{
	EA=1;	//��CPU���ж�
    EX1=1;	//���ⲿ�ж�����λ
	IT1=1;	//�½��ش���		
}

void dz(u8 dat)	
{
	u8 a;
	SCK=0;
	RCK=0;	//���͵�ƽ
	for(a=0;a<8;a++) //��������λ���뵽�Ĵ�����
	{
	SER=dat>>7;
	dat<<=1;
	SCK=1;
	_nop_();//��ʱһ����������
	_nop_();
	SCK=0;   //���͵�ƽ��Ϊ��һ����������׼��
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
	delay(100);   //��������
	if(k3==0)
	{
		led=~led;
	}	
}

void zhongduan1() interrupt 2	  //�ⲿ�ж�1���жϺ�Ϊ2��
{
	delay(100);   //��������
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