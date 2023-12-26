#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "public.h"


void lcd1602_init();
void lcd1602_clear();
void lcd1602_write_cmd( u8 cmd);
void lcd1602_write_data( u8 dat);
void lcd1602_write_string( u8 x, u8 y,u8 *s);
void lcd1602_write_char( u8 x, u8 y,u8 dat);
void lcd1602_write_int( u8 x, u8 y,int dat,u8 len);
void lcd1602_write_hex( u8 x, u8 y,u16 dat,u8 len);


#endif