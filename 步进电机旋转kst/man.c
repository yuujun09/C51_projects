#include <reg52.h>

sbit KEY_IN_1 = P2^4;
sbit KEY_IN_2 = P2^5;
sbit KEY_IN_3 = P2^6;
sbit KEY_IN_4 = P2^7;
sbit KEY_OUT_1 = P2^3;
sbit KEY_OUT_2 = P2^2;
sbit KEY_OUT_3 = P2^1;
sbit KEY_OUT_4 = P2^0;
signed long beats = 0;	//储存节拍数

unsigned char code KeyCodeMap[4][4] = {		//标准键盘映射
	{ 0X31,0X32,0X33,0X26},
	{ 0X34,0X35,0X36,0X25},
	{ 0X37,0X38,0X39,0X28},
	{ 0X30,0X1B,0X0D,0X27}	 
	};

unsigned char KeySta[4][4] = {					//存储矩阵按键状态
	{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};


void StartMotor(unsigned long angle);
void KeyDriver();

void main()
{
	EA = 1;
	TMOD = 0X01;
	TH0 = 0XFC;		 //定时1ms
	TL0 = 0X67;
	ET0 = 1;
	TR0 = 1;

	while(1)
	{
		KeyDriver();	   //按键驱动函数
	}

	
}

void StartMotor(signed long angle)
{
	EA = 0;								  //因为赋值为串口传输，中断可能会打断赋值，导致借位？
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
	static bit dirMotor = 0;						//记录电机旋转方向
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



void KeyDriver()	 //检测按键状态
{
	unsigned char i,j;
	static unsigned char backup [4][4] = {		   //存储按键上一时刻状态
	{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(backup[i][j] != KeySta[i][j])		 //状态发生改变
			{
				if(backup[i][j] == 0)				  //上一个时刻按键处于按下状态，并且状态发生改变，即当前按键已弹起
				{
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j] = KeySta[i][j];		  //将当前按键状态赋给backup
			}
		}
}

void TurnMotor()
{
	unsigned char tmp;	
	static unsigned char index = 0;
	unsigned char code BeatCode[8] = {				 //28BYJ-48步进电机8拍
	0X0E,0X0C,0X0D,0X09,0X0B,0X03,0X07,0X06};
	if(beats != 0)
	{
		if(beats > 0)
		{
			index++;				//电机正方向运行
			index = index & 0x07;	
			beats--;
		}
		else
		{						   //电机逆方向运行
			index--;
			index = index & 0x07;
			beats++;
		}
		tmp = P1;
		tmp = tmp & 0xf0;			 //我们只使用P1口低4位，防止影响P1口高4位
		tmp = tmp | BeatCode[index];
		P1 = tmp;
	}
	else				   //节拍数为0时停止运行
	{
		P1 = P1 | 0X0F;
	}

}

void KeyScan()
{
	unsigned char i;
	static unsigned char keyout = 0;	 //矩阵按键扫描输出索引
	static unsigned char keybuf[4][4] = {			//矩阵按键扫描缓冲区
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
	};

   	//将一行4个按键送入缓冲区
	keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
	keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
	keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
	keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
	//消抖并更新按键状态
	for(i=0;i<4;i++)
	{
		if ((keybuf[keyout][i] & 0x0f) == 0x00)
		{	//连续4次扫描值为0，认为按键已稳定按下
			KeySta[keyout][i] = 0;
		}
		else if ((keybuf[keyout][i] & 0x0f) == 0x0f)
		{	//连续4次扫描值为1，认为按键以稳定弹起
			KeySta[keyout][i] = 1;
		}
	}

	//执行下一次扫描输出
	keyout++;		//输出索引值递增
	keyout &= 0x03; //索引值加到4归零
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
	static bit div = 0; //用于定时器计数
	
	TH0 = 0XF8;
	TL0 = 0XCD;

	KeyScan();
	div = ~div;
	if(div == 0)
	{
		TurnMotor();
	}
}