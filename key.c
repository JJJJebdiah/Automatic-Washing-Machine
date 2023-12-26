#include "key.h"

#define KEY_MATRIX_PORT P1

sbit row1=KEY_MATRIX_PORT^7;
sbit row2=KEY_MATRIX_PORT^6;
sbit row3=KEY_MATRIX_PORT^5;
sbit row4=KEY_MATRIX_PORT^4;
sbit col1=KEY_MATRIX_PORT^3;
sbit col2=KEY_MATRIX_PORT^2;
sbit col3=KEY_MATRIX_PORT^1;
sbit col4=KEY_MATRIX_PORT^0;

//sbit key[4]={P3^1,P3^0,P3^2,P3^3};
//key[0]=P3^1;
//key[1]=P3^0;
//key[2]=P3^2;
//key[3]=P3^3;
/*
sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;
*/
u8 key_matrix_state=0;

u8 key_independent_scan_rising()
{
	/*
	上升沿，即按键松开瞬间
	此函数需要在定时器中断中调用，间隔时间为10ms
    return:NONE_PRESS-未被按下，KEY1_PRESS,KEY2_PRESS,KEY3_PRESS,KEY4_PRESS为按下的按键
	*/
	
//	static u8 key_state=0;
//	u8 i;
//	if(key_state==0)//上个时刻无按键被按下
//	{
//		for(i=0;i<4;i++)
//		{
//			if(key[i]==0)
//			{
//				key_state=i+1;
//				return NONE_PRESS;
//			}
//		}
		return NONE_PRESS;
		/*
		if(key1==0)//key1被按下
		{
			key_state=1;
			return NONE_PRESS;
		}
		if(key2==0)
		{
			key_state=2;
			return NONE_PRESS;
		}
		if(key3==0)
		{
			key_state=3;
			return NONE_PRESS;
		}
		if(key4==0)
		{
			key_state=4;
			return NONE_PRESS;
		}
		return NONE_PRESS;*/
//	}
	//上个时刻有按键被按下
//	if(key[key_state-1]==0)//按键仍被按下
//	{
//		return NONE_PRESS;
//	}
//	//按键被松开
//	i=key_state;
//	key_state=0;
//	return i;
	/*switch(key_state)
	{
		case 1:
			if(key1==1)
			{
				key_state=0;
				return KEY1_PRESS;
			}
			break;
		case 2:
			if(key2==1)
			{
				key_state=0;
				return KEY2_PRESS;
			}
			break;
		case 3:
			if(key3==1)
			{
				key_state=0;
				return KEY3_PRESS;
			}
			break;
		case 4:
			if(key4==1)
			{
				key_state=0;
				return KEY4_PRESS;
			}
			break;
	}
	return 0;//按键未松开*/


}

//u8 key_independent_scan_falling()
//{
//	/*
//	下降沿，即按键按下瞬间
//	此函数需要在定时器中断中调用，间隔时间为10ms
//    return:NONE_PRESS-未被按下，KEY1_PRESS,KEY2_PRESS,KEY3_PRESS,KEY4_PRESS为按下的按键
//	*/
//	static u8 key_state=0;
//	if(key_state==0)//上个时刻无按键被按下
//	{
//		if(key1==0)//key1被按下
//		{
//			delay_ms_11M(1);//消抖
//			if(key1==0)//确实被按下
//			{
//				key_state=1;
//				return KEY1_PRESS;
//			}
//		}
//		if(key2==0)
//		{
//			delay_ms_11M(1);//消抖
//			if(key2==0)
//			{
//				key_state=2;
//				return KEY2_PRESS;
//			}
//		}
//		if(key3==0)
//		{
//			delay_ms_11M(1);//消抖
//			if(key3==0)
//			{
//				key_state=3;
//				return KEY3_PRESS;
//			}
//		}
//		if(key4==0)
//		{
//			delay_ms_11M(1);//消抖
//			if(key4==0)
//			{
//				key_state=4;
//				return KEY4_PRESS;
//			}
//		}
//		return NONE_PRESS;
//	}
//	//上个时刻有按键被按下
//	return NONE_PRESS;
//}

u8 key_column_scan()
{
	/*
	按键矩阵列扫描
	return:0-未被按下
	*/
	if(col1==0)
	{
		return 1;
	}
	if(col2==0)
	{
		return 2;
	}
	if(col3==0)
	{
		return 3;
	}
	if(col4==0)
	{
		return 4;
	}
	return 0;
}

u8 key_row_scan(u8 row)
{
	u8 i=1,x;
	if(key_matrix_state==0)
	//上个时刻无按键被按下
	{
		x=key_column_scan();
		if(x==0)
		{
			//无按键被按下
			return NONE_PRESS;
		}
		key_matrix_state=x+4*row-4;
		return NONE_PRESS;
	}
	for(;i<5;i++)
	{
		if(key_matrix_state==i+4*row-4)
		{
			if(key_column_scan()==i)
			{
				//S(i+4*row-4)仍被按下
				return NONE_PRESS;
			}
			else
			{
				//S(i+4*row-4)被松开
				key_matrix_state=0;
				return i+4*row-4;
			}
		}
	}
	return NONE_PRESS;
}

u8 key_matrix_scan_rising()
{
	/*
	上升沿，即按键松开瞬间
	此函数需要在定时器中断中调用，间隔时间为10ms
    return:NONE_PRESS-未被按下 1-16为按下的按键
	*/
	u8 re;
	KEY_MATRIX_PORT=0x7f;//第一行低电平，其他行高电平
	re=key_row_scan(1);//扫描第一行
	if(re!=NONE_PRESS)
	{
		return re;//返回按键号
	}
	KEY_MATRIX_PORT=0xbf;//第二行低电平，其他行高电平
	re=key_row_scan(2);
	if(re!=NONE_PRESS)
	{
		return re;
	}
	/*KEY_MATRIX_PORT=0xdf;//第三行低电平，其他行高电平
	re=key_row_scan(3);
	if(re!=NONE_PRESS)
	{
		return re;
	}
	KEY_MATRIX_PORT=0xef;//第四行低电平，其他行高电平
	re=key_row_scan(4);
	if(re!=NONE_PRESS)
	{
		return re;
	}*/
	return NONE_PRESS;

}

