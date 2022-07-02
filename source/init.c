#include "init.h"
#include "callback.h"
code unsigned long SysClock = 11059200;

void disp_init()
{
    LedPrint(0);
    DisplayerInit();
    LedPrint(0);
    SetDisplayerArea(0, 7);
    Seg7Print(36, 36, 36, 36, 36, 36, 36, 36); // display off
}

void date_init(u_byte y, u_byte mon, u_byte d, u_byte h, u_byte min, u_byte s, u_byte week)
{
    struct_DS1302_RTC time;
    time.year = y;
    time.month = mon;
    time.day = d;
    time.hour = h;
    time.minute = min;
    time.second = s;
    time.week = week;

    DS1302Init(time);
}

void global_init()
{
#if 1
    // clock_cur = RTC_Read();
    // ADC and Navigation button
    AdcInit(ADCexpEXT);

    // Buzzer
    BeepInit();

    // Display & LEDs
    disp_init();

    // Persistent Clock & Storage
    date_init(2022, 7, 1, 0, 0, 0, 1);

    // Buttons
    Key_Init();

    // Vibration detector
    VibInit();
#endif
}
