#include "lcd1602.h"

#define LCD1602_E P27//E=1:允许读写，E=0:禁止读写
#define LCD1602_RW P25//RW=0:写，RW=1:读
#define LCD1602_RS P26//RS=0:写指令，RS=1:写数据
#define LCD1602_DATA_PORT P0//数据总线

void lcd1602_write_cmd(u8 cmd)
{
    /*
        写指令
    */
    LCD1602_E = 0;
    LCD1602_RS = 0;//写指令
    LCD1602_RW = 0;//写
    LCD1602_DATA_PORT = cmd;//加载命令
    delay_ms_11M(1);
    LCD1602_E = 1;//允许读写
    delay_ms_11M(1);
    LCD1602_E = 0;//读写结束
}

void lcd1602_write_data(u8 dat)
{
    /*
        写数据
    */
    LCD1602_E = 0;
    LCD1602_RS = 1;//写数据
    LCD1602_RW = 0;//写
    LCD1602_DATA_PORT = dat;//加载数据
    delay_ms_11M(1);
    LCD1602_E = 1;//允许读写
    delay_ms_11M(1);
    LCD1602_E = 0;//读写结束
}
void lcd1602_init()
{
    lcd1602_write_cmd(0x38);	// 数据总线8位，显示2行，5*7点阵字符
    lcd1602_write_cmd(0x0c);	// 显示开，光标关，光标不闪烁
    lcd1602_write_cmd(0x06);	// 每次写入后光标右移，显示屏不移动
    lcd1602_write_cmd(0x01);	// 清屏指令，光标归0，AC归0
}

u8 lcd1602_set_cursor(u8 x,u8 y)
{
    /*
        设置光标位置
        x:列0-15
        y:行0-1
        设置失败返回0
    */
    if(y>1)
        return 0;
    if(y==0)
    {
        if(x>31) return 0;
        if(x<16)
            lcd1602_write_cmd(0x80+x);
        else
            lcd1602_write_cmd(0xc0+x-16);
    }
    else
    {
        if(x>15) return 0;
        lcd1602_write_cmd(0xc0+x);
    }
    return 1;
}

void lcd1602_write_string(u8 x,u8 y,u8 *s)
{
    /*
        写字符串
        x:列0-15
        y:行0-1
    */
    u8 i=0;
    if(lcd1602_set_cursor(x,y)==0)//设置光标位置
        return;
    while(*s!='\0')
    {
        lcd1602_write_data(*s);//逐个显示字符
        i++;
        s++;
        if(i==16-x)
        {
            if(y==0)
                lcd1602_write_cmd(0xc0);//换到第二行
            else
                return;
        }
    }
}

void lcd1602_write_char(u8 x,u8 y,u8 dat)
{
    /*
        写字符
        x:列0-15
        y:行0-1
    */
    if(lcd1602_set_cursor(x,y)==0)
        return;
    lcd1602_write_data(dat);
}

void lcd1602_clear()
{
    /*
        清屏
    */
    lcd1602_write_cmd(0x01);
}

u16 lcd1602_pow(u16 x,u16 y)
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

void lcd1602_write_int( u8 x, u8 y,int dat,u8 len)
{
    /*
        写整数,可以是负数,最大值32767
        x:列0-15
        y:行0-1
        len:数字长度,最大5
     */
    u16 new_dat=0;
    u8 i=0;
    if(lcd1602_set_cursor(x,y)==0)//设置光标位置
        return;
    if(len>5) len=5;
    if(dat>=0)
    {
        new_dat=dat;
    }
    else
    {
        lcd1602_write_char(x,y,'-');//负数显示负号
        new_dat=-dat;
        x++;
    }
    for(i=len;i>0;i--)
    {
        if(len-i==16-x)
        {
            if(y==0)
                lcd1602_write_cmd(0xc0);//换到第二行
            else
                return;
        }
        lcd1602_write_data(new_dat/lcd1602_pow(10,i-1)%10+'0');//逐个显示数字
    }

}

void lcd1602_write_hex( u8 x, u8 y,u16 dat,u8 len)
{
    /*写十六进制数
    len:数字长度,最大4
    */
   u8 i=0,dat1=0;
    if(lcd1602_set_cursor(x,y)==0)
        return;
    if(len>4) len=4;
    for(i=len;i>0;i--)
    {
        if(len-i==16-x)
        {
            if(y==0)
                lcd1602_write_cmd(0xc0);
            else
                return;
        }
        dat1=dat/lcd1602_pow(16,i-1)%16;
        if(dat1<10)
            lcd1602_write_data(dat1+'0');
        else
            lcd1602_write_data(dat1-10+'A');
    }
}