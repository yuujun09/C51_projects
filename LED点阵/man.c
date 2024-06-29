#include "reg51.h"
#include "intrins.h"
typedef unsigned char u8;
typedef unsigned int u16;
sbit SER=P3^4;
sbit SCK=P3^6;
sbit RCK=P3^5;

u8 col[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xde};
//λѡ���ڼ�������  :��һ�е��ڰ���		 			  

u8 row[]={0x00,0x00,0x3e,0x41,0x41,0x41,0x3e,0x00};
//��ѡ���ڼ�������			 0010 0001

void delay(u8 i)
{
	while(i--);
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
	u8 i;
	P0=0x7f;
	while(1)
	{
		for(i=0;i<8;i++)
		{
		P0=col[i];
		dz(row[i]);
		delay(100);
		dz(0x00);		
		 }		
	}
}