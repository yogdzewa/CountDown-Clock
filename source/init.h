#ifndef _INIT_
#define _INIT_
#include "common_include.h"

extern code unsigned long SysClock;
void disp_init();
void date_init(u_byte y, u_byte mon, u_byte d, u_byte h, u_byte min, u_byte s, u_byte week);
void FM_init();
void global_init();

#endif //_INIT_ 