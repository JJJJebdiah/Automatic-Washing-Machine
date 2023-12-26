#include "public.h"
#include "lcd1602.h"
#include "key.h"
#include "serial.h"
#include "stepper.h"
#include "buzzer.h"
#include "ired.h"




//函数

void start_washing(); //开始洗衣函数
void stop_washing(); //停止洗衣函数
void display(); //显示函数
void send_data(); //串口发送数据函数
void AWM_init(); //洗衣机初始化函数