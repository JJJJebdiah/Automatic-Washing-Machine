#include "serial.h"

void serial_init_9600(bit flag)//9600bps@11.0592MHz
{
	/*
	串口初始化函数,flag=1,打开串口中断,即接收数据
	*/
	if(flag==1)
	{
		ES=1;				//打开串口中断
		EA=1;				//打开总中断
	}
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}

/*void serial_interrupt() interrupt 4
{
	if(RI==1)
	{
		RI=0;
	}
}*/
void serial_send_char(u8 d)
{
	/*
	发送一个字节,可以是字符
	*/
	SBUF=d;
	while(TI==0);//等待发送完成
	TI=0;
}

void serial_send_string(u8* s)
{
	/*
	发送一个字符串
	*/
	while(*s!='\0')
	{
		serial_send_char(*s);
		s++;
	}
}

u16 serial_pow(u16 x,u16 y)
{
	/*
        计算x的y次方
    */
    u16 i=0,s=1;
    for(i=0;i<y;i++)
    {
        s*=x;
    }
    return s;
}

void serial_send_int(int dat,u8 len)
{
	/*
	发送一个整数,可以是负数,最大值32767
	len:数字长度,最大5
	*/
	u16 new_dat=0;
	u8 i;
	if(len>5) len=5;
	if(dat>=0)
    {
        new_dat=dat;
    }
    else
    {
        serial_send_char('-');//负数发送负号
        new_dat=-dat;
    }

	for(i=len;i>0;i--)
    {
        serial_send_char(new_dat/serial_pow(10,i-1)%10+'0');//逐个发送数字
    }

}