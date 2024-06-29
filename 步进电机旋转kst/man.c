#include <reg52.h>

sbit KEY_IN_1 = P2^4;
sbit KEY_IN_2 = P2^5;
sbit KEY_IN_3 = P2^6;
sbit KEY_IN_4 = P2^7;
sbit KEY_OUT_1 = P2^3;
sbit KEY_OUT_2 = P2^2;
sbit KEY_OUT_3 = P2^1;
sbit KEY_OUT_4 = P2^0;
signed long beats = 0;	//���������

unsigned char code KeyCodeMap[4][4] = {		//��׼����ӳ��
	{ 0X31,0X32,0X33,0X26},
	{ 0X34,0X35,0X36,0X25},
	{ 0X37,0X38,0X39,0X28},
	{ 0X30,0X1B,0X0D,0X27}	 
	};

unsigned char KeySta[4][4] = {					//�洢���󰴼�״̬
	{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};


void StartMotor(unsigned long angle);
void KeyDriver();

void main()
{
	EA = 1;
	TMOD = 0X01;
	TH0 = 0XFC;		 //��ʱ1ms
	TL0 = 0X67;
	ET0 = 1;
	TR0 = 1;

	while(1)
	{
		KeyDriver();	   //������������
	}

	
}

void StartMotor(signed long angle)
{
	EA = 0;								  //��Ϊ��ֵΪ���ڴ��䣬�жϿ��ܻ��ϸ�ֵ�����½�λ��
	beats = (angle*4076)/360;
	EA = 1;
}

void StopMotor()
{
	EA = 0;
	beats = 0;
	EA = 1;
}

void KeyAction(signed char keycode)
{
	static bit dirMotor = 0;						//��¼�����ת����
	if(keycode >= 0x30 && keycode <= 0x39)
	{
		if(dirMotor == 0)
		{
			StartMotor(360*(keycode - 0x30));
		}
		else
		{
			StartMotor(-360*(keycode - 0x30));		
		}
	}
	else if(keycode == 0x26)
	{
		dirMotor = 0;
	}
	else if(keycode == 0x28)
	{
		dirMotor = 1;
	}
	else if(keycode == 0x25)
	{
		StartMotor(90);
	}
	else if(keycode == 0x27)
	{
		StartMotor(-90);
	}
	else if(keycode == 0x1B)
	{
		StopMotor();
	}
}



void KeyDriver()	 //��ⰴ��״̬
{
	unsigned char i,j;
	static unsigned char backup [4][4] = {		   //�洢������һʱ��״̬
	{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(backup[i][j] != KeySta[i][j])		 //״̬�����ı�
			{
				if(backup[i][j] == 0)				  //��һ��ʱ�̰������ڰ���״̬������״̬�����ı䣬����ǰ�����ѵ���
				{
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j] = KeySta[i][j];		  //����ǰ����״̬����backup
			}
		}
}

void TurnMotor()
{
	unsigned char tmp;	
	static unsigned char index = 0;
	unsigned char code BeatCode[8] = {				 //28BYJ-48�������8��
	0X0E,0X0C,0X0D,0X09,0X0B,0X03,0X07,0X06};
	if(beats != 0)
	{
		if(beats > 0)
		{
			index++;				//�������������
			index = index & 0x07;	
			beats--;
		}
		else
		{						   //����淽������
			index--;
			index = index & 0x07;
			beats++;
		}
		tmp = P1;
		tmp = tmp & 0xf0;			 //����ֻʹ��P1�ڵ�4λ����ֹӰ��P1�ڸ�4λ
		tmp = tmp | BeatCode[index];
		P1 = tmp;
	}
	else				   //������Ϊ0ʱֹͣ����
	{
		P1 = P1 | 0X0F;
	}

}

void KeyScan()
{
	unsigned char i;
	static unsigned char keyout = 0;	 //���󰴼�ɨ���������
	static unsigned char keybuf[4][4] = {			//���󰴼�ɨ�軺����
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
	};

   	//��һ��4���������뻺����
	keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
	keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
	keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
	keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
	//���������°���״̬
	for(i=0;i<4;i++)
	{
		if ((keybuf[keyout][i] & 0x0f) == 0x00)
		{	//����4��ɨ��ֵΪ0����Ϊ�������ȶ�����
			KeySta[keyout][i] = 0;
		}
		else if ((keybuf[keyout][i] & 0x0f) == 0x0f)
		{	//����4��ɨ��ֵΪ1����Ϊ�������ȶ�����
			KeySta[keyout][i] = 1;
		}
	}

	//ִ����һ��ɨ�����
	keyout++;		//�������ֵ����
	keyout &= 0x03; //����ֵ�ӵ�4����
	switch(keyout)
	{
		case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
		case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
		case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
		case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
	}
}


void InterruptTimer0() interrupt 1
{	
	static bit div = 0; //���ڶ�ʱ������
	
	TH0 = 0XF8;
	TL0 = 0XCD;

	KeyScan();
	div = ~div;
	if(div == 0)
	{
		TurnMotor();
	}
}