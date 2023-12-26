#ifndef __KEY_H__
#define __KEY_H__

#include "public.h"

#define NONE_PRESS 0
#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS 4


u8 key_independent_scan_rising();
u8 key_independent_scan_falling();
u8 key_matrix_scan_rising();


#endif