#include "buzzer.h"

#define BEEP P25

void buzzer_play_11M(u16 xms, u16 freq)
{
    /*
        xms:持续时间ms
        freq:频率0-5000
    */
    u16 i;
    if(freq==0||freq>5000||xms==0)
        return;
    for(i=0;i<xms*1000/freq;i++)
    {

        BEEP = !BEEP;
        delay_100us_11M(5000/freq);
        BEEP = !BEEP;
        delay_100us_11M(5000/freq);
    }
    BEEP = 0;

}

void buzzer_play_12M(u16 xms, u16 freq)
{

        //xms:持续时间ms
        //freq:频率0-5000
    u16 i;
    if(freq==0||freq>5000||xms==0)
        return;
    for(i=0;i<xms*1000/freq;i++)
    {
        BEEP = !BEEP;
        delay_100us_12M(5000/freq);
        BEEP = !BEEP;
        delay_100us_12M(5000/freq);
    }
    BEEP = 0;

}