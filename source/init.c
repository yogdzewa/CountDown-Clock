#include "init.h"
#include "callback.h"
#include "help_func.h"
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
    TIME_RELD_H = M24C02_Read(0);
    TIME_RELD_M = M24C02_Read(1);
    TIME_RELD_S = M24C02_Read(2);
    TIME_REST_M = M24C02_Read(3);
    TIME_LIMIT_ALLSEC = TIME_RELD_H * 3600;
    TIME_LIMIT_ALLSEC += TIME_RELD_M * 60;
    TIME_LIMIT_ALLSEC += TIME_RELD_S;

    DS1302Init(time);
}

void global_init()
{
    // for uart1 init
    Uart1Init(115200);

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
}
