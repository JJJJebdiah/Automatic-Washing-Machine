#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include<STC89C5xRC.H>

typedef unsigned char u8;
typedef unsigned int u16;

void delay_ms_12M(u16 xms);
void delay_ms_11M(u16 xms);
void delay_100us_11M(u16 xus);
void delay_100us_12M(u16 xus);
void delay_10us();
void init_timer0_5ms();//5毫秒@11.0592MHz
void init_timer0_1ms();//1毫秒@11.0592MHz


#endif
