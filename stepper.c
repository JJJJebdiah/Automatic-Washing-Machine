#include "stepper.h"

u8 step[8]={0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01};//步进电机步进序列

#define STEPPER_PORT P1//低四位

void stepper_motor(u8 dir)
{
    /*dir:转动方向，0表示顺时针，1表示逆时针*/
    static u8 i=0;
    STEPPER_PORT &= 0xf0;
    STEPPER_PORT |= step[i];
    if(dir==0)
        i++;
    else i--;
    if(i==8)
    {
        i=0;
    }
    else if(i==255)
    {
        i=7;
    }
}
