#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "public.h"

void serial_init_9600(bit flag);
void serial_send_char(u8 d);
void serial_send_string(u8* s);
void serial_send_int(int dat,u8 len);


#endif