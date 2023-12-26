#include "digital_tube.h"

u8 led[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//0-9

void dt_dynamic_8CC(u8 location,u8 dat)
{
    /*
    共阴
    138 Y0-Y7接8-1
    location:位选0-7
    dat:显示的数字0-9
    */

   if(dat>9)
		return;
	DT_PORT=0x00;
	switch(location)
	{
		case 7:
			C=0;B=0;A=0;
			break;
		case 6:
			C=0;B=0;A=1;
			break;
		case 5:
			C=0;B=1;A=0;
			break;
		case 4:
			C=0;B=1;A=1;
			break;
		case 3:
			C=1;B=0;A=0;
			break;
		case 2:
			C=1;B=0;A=1;
			break;
		case 1:
			C=1;B=1;A=0;
			break;
		case 0:
			C=1;B=1;A=1;
			break;
		default :return;
			break;
	}
	DT_PORT=led[dat];
}