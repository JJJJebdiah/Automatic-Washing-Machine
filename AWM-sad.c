#include "AWM.h"

#define LOW_WATER 1
#define HIGH_WATER 2

//状态变量
u16 time = 60; //洗衣机运行时间
bit mode=0; //洗衣机模式，0表示标准模式，1表示轻柔模式
bit start=0; //洗衣机启动状态，0表示停止，1表示正在运行
bit flag=0;//中断时存储start状态
u16 speed[2]={500,1000};//步进机转速，表示间隔多少100us步进一次
u8 direct=0;//0-9记录转动秒数，小于5时顺时针
u8 water=0;//水位信号，0表示正常，LOW_WATER表示低水位，HIGH_WATER表示高水位
u8 is_water=0;//是否放/加水，0表示不放/加水，1表示放/加水

//变量
u8* stepper_string="speed:XXXX";
u16 count = 0,run=0,water_time=0;//run表示洗衣机运行时间,count表示程序运行时间,water_time表示放/加水时间
u8 ired_cmd;//红外数据控制码
struct data_from_SBUF
{
    u8 d[10];
    u8 len;
} dfs;


void display()
{
    lcd1602_write_string(0,0,"mode:  start:   time:   s");
    lcd1602_write_int(5,0,mode,1);
    lcd1602_write_int(13,0,start,1);
    lcd1602_write_int(21,0,time,3);
}
void send_data()
{
    display();
    serial_send_string("mode: ");
    serial_send_int(mode,1);
    serial_send_string(" start: ");
    serial_send_int(start,1);
    serial_send_string(" time: ");
    serial_send_int(time,3);
    serial_send_string(" s\r\n");
}
void clear_dfs()
{
    u8 i;
    for(i=0;i<dfs.len;i++)
    {
        dfs.d[i]=0;
    }
    dfs.len=0;
}
void AWM_init()
{
    lcd1602_init();//初始化lcd1602
    init_timer0_5ms();//初始化定时器0
    serial_init_9600(1);//初始化串口
    ired_init();//初始化红外接收器
    clear_dfs();

}
void do_dfs()
{
    if(dfs.len==7)//$mode=0;
    {
        if(dfs.d[1]=='m'&&dfs.d[2]=='o'&&dfs.d[3]=='d'&&dfs.d[4]=='e'&&dfs.d[5]=='='&&dfs.d[6]=='0')
        {
            mode=0;
            send_data();
            return;
        }
        else if(dfs.d[1]=='m'&&dfs.d[2]=='o'&&dfs.d[3]=='d'&&dfs.d[4]=='e'&&dfs.d[5]=='='&&dfs.d[6]=='1')
        {
            mode=1;
            send_data();
            return;
        }
        else
        {
            return;
        }
    }
    if(dfs.len==8)//$start=0;
    {
        if(dfs.d[1]=='s'&&dfs.d[2]=='t'&&dfs.d[3]=='a'&&dfs.d[4]=='r'&&dfs.d[5]=='t'&&dfs.d[6]=='='&&dfs.d[7]=='0')
        {
            start=0;
            flag=0;
            send_data();
            return;
        }
        else if(dfs.d[1]=='s'&&dfs.d[2]=='t'&&dfs.d[3]=='a'&&dfs.d[4]=='r'&&dfs.d[5]=='t'&&dfs.d[6]=='='&&dfs.d[7]=='1')
        {
            start=1;
            flag=1;
            send_data();
            return;
        }
        else
        {
            return;
        }
    }
    if(dfs.len==4)//$add;  $sub;
    {
        if(dfs.d[1]=='a'&&dfs.d[2]=='d'&&dfs.d[3]=='d')
        {
            time+=15;
            send_data();
            return;
        }
        else if(dfs.d[1]=='s'&&dfs.d[2]=='u'&&dfs.d[3]=='b')
        {
            if(time<15)
            {
                time=0;
                start=0;
                flag=0;
            }
            else
            {
                time-=15;
            }
            send_data();
            return;
        }
        else
        {
            return;
        }
    }
}

void check_dfs()
{
    if(dfs.d[0]!='$'&&dfs.len==0)
    {
        dfs.d[0]=0;
        return;
    }
    if(dfs.d[dfs.len]==';')
    {
        do_dfs();
        clear_dfs();
        return;
    }
    dfs.len++;
    if(dfs.len>=10)
    {
        clear_dfs();
        return;
    }
}
void serial_interrupt() interrupt 4
{
    if(RI==1)
    {
        RI=0;
        dfs.d[dfs.len]=SBUF;
        check_dfs();
    }
}

void independent_keys()//独立按键扫描
{
    u8 key=0;
    key=key_independent_scan_rising();
    switch(key)
    {
        case NONE_PRESS:
            break;
        case KEY1_PRESS:
            //启动/暂停按钮按下
            if(start==1)
            {
                start=0;
                flag=0;
            }
            else
            {
                start=1;
                flag=1;
            }
            send_data();
            break;

    }

}

void matrix_keys()
{
    u8 key=0;
    key=key_matrix_scan_rising();
    switch(key)
    {
        case NONE_PRESS:
            return;
            break;
        case 1:
            //S1模式选择按钮按下
            buzzer_play_11M(100,1000);
            if(mode==1)
            {
                //标准模式
                mode=0;
            }
            else
            {
                //轻柔模式
                mode=1;
            }
            send_data();
            break;
        case 2:
            //增时按钮按下
            time+=15;
            buzzer_play_11M(100,1000);
            send_data();
            break;
        case 3:
            //减时按钮按下
            buzzer_play_11M(100,1000);
            if(time<15)
            {
                time=0;
                start=0;
                flag=0;
            }
            else
            {
                time-=15;
            }
            send_data();
            break;
        case 4:
            //控制模式切换按钮按下
            break;
        case 5:
            //低水位按钮按下
            if(water==LOW_WATER) water=0;
            else if(water==0)
                water=LOW_WATER;
            break;
        case 6:
            //高水位按钮按下
            if(water==HIGH_WATER) water=0;
            else if(water==0)
                water=HIGH_WATER;
            break;
    }
}

u8 water_interrupt()
{
    start=0;//停止洗衣机
    if(count==200)
        water_time++;
    if(water_time==0)
    {
        lcd1602_clear();
        water_time=1;
        if(water==LOW_WATER)
        {
            //加水
            lcd1602_write_string(0,0,"adding water...");
        }
        else if(water==HIGH_WATER)
        {
            //放水
            lcd1602_write_string(0,0,"watering...");
        }
    }
    if(water_time%10==0)//10s后仍异常
    {
        lcd1602_clear();
        lcd1602_write_string(0,0,"water erro!");
        if(water==LOW_WATER)
        {
            //放水
            lcd1602_write_string(0,1,"low water!");
        }
        else if(water==HIGH_WATER)
        {
            //加水
            lcd1602_write_string(0,1,"high water!");
        }
        //开启蜂鸣器
        return 1;
    }
    return 0;
}

void ired()
{
    /*处理红外数据*/
    if(ired_cmd==0x45)
    {
        //启动/暂停按钮按下
        if(start==1)
        {
            start=0;
            flag=0;
        }
        else
        {
            start=1;
            flag=1;
        }
        send_data();
    }
    else if(ired_cmd==0x46)
    {
        //模式选择按钮按下
        buzzer_play_11M(100,1000);
        if(mode==1)
        {
            //标准模式
            mode=0;
        }
        else
        {
            //轻柔模式
            mode=1;
        }
        send_data();
    }
    else if(ired_cmd==0x15)
    {
        //减时按钮按下
        buzzer_play_11M(100,1000);
        if(time<15)
        {
            time=0;
            start=0;
            flag=0;
        }
        else
        {
            time-=15;
        }
        send_data();
    }
    else if(ired_cmd==0x09)
    {
        //增时按钮按下
        time+=15;
        buzzer_play_11M(100,1000);
        send_data();
    }

}

void timer0_interrupt() interrupt 1  //0表示外部中断0，1表示定时器0中断
//2表示外部中断1，3表示定时器1中断，4表示串口中断
{
    /*5ms*/
    static u8 re=0;//接收water_interrupt()的返回值
    count++;
    TH0 = 0xee; // 定时器0的高8位
    TL0 = 0x00; // 定时器0的低8位
    //每10ms扫描一次按键
    if(count%2==0)
    {
        if(water==0)//水位正常
            independent_keys();//独立按键扫描
        if(start==0)
            matrix_keys();//矩阵按键扫描
    }
    if(water==0)
    {
        ired_cmd=ired_get_cmd();//获得红外数据控制码
        if(ired_cmd!=0)
        {
            ired();//处理红外数据
        }
    }
    if(water==0 && is_water==1)
    {
        //水位正常，停止放/加水
        is_water=0;
        water_time=0;
        re=0;
        start=flag;//恢复洗衣机启动状态
        lcd1602_clear();
        send_data();
    }
    if(water!=0)
    {
        //水位异常，放/加水
        if(re!=1)
            re=water_interrupt();
        else
            buzzer_play_11M(4,1000);
        is_water=1;
    }
    if(count==200) count=0;
    /*if(start==1&&run%100==0)
    {
        stepper_motor(0);
    }*/
    if(start==1)
        run++;
    //每1s更新一次显示数据
    if(run == 200)
    {
        run = 0;
        direct++;
        if(direct==10)
            direct=0;
		if(time < 2)
        {
            // 关闭洗衣机
			start=0;
            time=0;
            flag=0;
            send_data();
            buzzer_play_11M(500,1000);
			return;

        }
        time--;
        send_data();
    }
}


void stop_washing()
{
    while(start!=1);
}

void start_washing()
{
    while(start==1)
    {
		if(start==0)
			break;
        if(direct<5)
        {
            stepper_motor(1);
        }
        else
            stepper_motor(0);
        delay_ms_11M(speed[mode]);
	}
}


