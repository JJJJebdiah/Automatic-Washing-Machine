#include "ired.h"
#include "lcd1602.h"

sbit IRED=P3^2;//接收端口

u8 cmd_flag=0,addr_flag=0;//数据接收标志位,0:未接收，1:接收完成
u8 ired_data[4];//存储接收到的数据(地址码、地址反码、数据码、数据反码)，为十六进制

void ired_init(void)
{
    /*
        初始化红外接收器，使用外部中断0
    */
	IT0=1;	//下降沿触发
	EX0=1;	//打开中断0
	EA=1;	//打开总中断
	IRED=1;
    PX0=1;//高优先级
}


u8 ired_get_cmd()
{
    /*获得控制码*/
    if(cmd_flag==1)
    {
        cmd_flag=0;
        return ired_data[2];
    }
    else
        return 0;
}

u8 ired_get_addr()
{
    /*获得地址码*/
    if(addr_flag==1)
    {
        addr_flag=0;
        return ired_data[0];
    }
    else
        return 0;
}


void ired_int() interrupt 0
{
    /*解码*/
    u16 t=0,high_time=0;
    u8 i,j;
    if(IRED==0)
    {
        t=1000;
        while(IRED==0&&t>0)//等待引导信号9ms低电平结束，若超过10ms则认为是干扰信号
        {
            delay_10us();
            t--;
            if(t==0)
            {
                addr_flag=0;
                cmd_flag=0;
                return;
            }
        }

        t=500;
        while(IRED==1&&t>0)//等待引导信号4.5ms高电平结束，若超过5ms则认为是干扰信号
        {
            delay_10us();
            t--;
            if(t==0)
            {
                addr_flag=0;
                cmd_flag=0;
                return;
            }
        }
        for(i=0;i<4;i++)//读取4个字节数据
        {
            for(j=0;j<8;j++)//读取8位数据
            {
                t=600;
                while(IRED==0&&t>0)//等待数据0/1前的0.56ms低电平结束，若超过1ms则认为是干扰信号
                {
                    delay_10us();
                    t--;
                    if(t==0)
                    {
                        addr_flag=0;
                        cmd_flag=0;
                        return;
                    }
                }
                while(IRED==1)//计算高电平时间
                {
                    delay_100us_11M(1);
                    high_time++;
                    if(high_time>20)//若超过2ms则认为是干扰信号
                    {
                        addr_flag=0;
                        cmd_flag=0;
                        return;
                    }
                }
                ired_data[i]>>=1;//右移一位
                if(high_time>=8)//若高电平时间大于0.8ms，则认为是数据1
                    ired_data[i]|=0x80;
                else ired_data[i]&=0x7f;//否则是数据0
                high_time=0;
            }
        }
        lcd1602_write_hex(0,0,0xac,2);
        //检验数据是否正确
        if(ired_data[0]!=~ired_data[1] || ired_data[2]!=~ired_data[3])
        {
            //数据错误
            for(i=0;i<4;i++)
                ired_data[i]=0;
            addr_flag=0;
            cmd_flag=0;
            return;
        }
        addr_flag=1;
        cmd_flag=1;//数据接收完成

    }
}