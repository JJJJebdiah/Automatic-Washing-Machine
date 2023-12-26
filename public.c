#include "public.h"

void delay_10us()
{
    u8 i=2;
    while(--i);
}

void delay_ms_12M(u16 xms)
{
    u16 i,j;
	for(i=0;i<xms;i++)
		for(j=0;j<120;j++);
}



void delay_ms_11M(u16 xms)
{
    u16 i,j;
    for(i=0;i<xms;i++)
        for(j=0;j<110;j++);
}

void delay_100us_11M(u16 xus)
{
    u16 i,j;
    for(i=0;i<xus;i++)
        for(j=0;j<11;j++);
}

void delay_100us_12M(u16 xus)
{
    u16 i,j;
    for(i=0;i<xus;i++)
        for(j=0;j<12;j++);
}

void init_timer0_5ms()//5毫秒@11.0592MHz
{
	TMOD &= 0xF0; // 清除定时器0的工作方式
    TMOD |= 0x01; // 选择定时器0工作在方式1
    //TH0 = 0xf8;TL0 = 0xcd;//2ms
	//TH0 = 0xdc;TL0 = 0x00;//10ms
	//TH0 = 0x4c;TL0 = 0x00;//50ms
	TH0 = 0xee;TL0 = 0x00;//5ms
    EA = 1; // 开启总中断
    ET0 = 1; // 开启定时器0中断
    TR0 = 1; // 开启定时器0
}

void init_timer0_1ms()//5毫秒@11.0592MHz
{
    TMOD &= 0xF0; // 清除定时器0的工作方式
    TMOD = 0x01; // 选择定时器0工作在方式1
    TH0 = 0xfc;TL0 = 0x66;//1ms
    EA = 1; // 开启总中断
    ET0 = 1; // 开启定时器0中断
    TR0 = 1; // 开启定时器0
}

