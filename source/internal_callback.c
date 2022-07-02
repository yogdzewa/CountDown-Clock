#include "callback.h"
#define CALLBACK_CNT 5
code callback_register all_callbacks[] = {
    // {enumEventSys1mS, on_timer_1ms},
    // {enumEventSys10mS, on_timer_10ms},
    {enumEventSys100mS, on_timer_100ms},
    // {enumEventSys1S, on_timer_1sec},

    {enumEventKey, __on_button},
    {enumEventNav, __on_nav},

    // {enumEventHall, on_sensor_hall},
    {enumEventVib, on_sensor_vib},

    // {enumEventIrRxd, on_ir_rx},
    // {enumEventUart1Rxd, on_uart1_rx},
    // {enumEventUart2Rxd, on_uart2_rx},

    {enumEventXADC, on_event_adc}};

void init_callbacks()
{
    u_byte n = CALLBACK_CNT;
    u_byte i = 0;
    for (; i < n; i++)
        SetEventCallBack(all_callbacks[i].event, all_callbacks[i].callback);
}

void __on_button()
{
    {
        u_byte key2act = GetKeyAct(enumKey2);
        if (key2act == enumKeyPress)
            on_btn2_down();
        else if (key2act == enumKeyRelease)
            on_btn2_up();
    }

    {
        u_byte key1act = GetKeyAct(enumKey1);
        if (key1act == enumKeyPress)
            on_btn1_down();
        else if (key1act == enumKeyRelease)
            on_btn1_up();
    }
}

void __on_nav()
{
    BUTTON_DISPATCH(enumAdcNavKey3, on_btn3_up, on_btn3_down);
    BUTTON_DISPATCH(enumAdcNavKeyCenter, on_nav_up, on_nav_down);
    BUTTON_DISPATCH(enumAdcNavKeyDown, on_downbtn_up, on_downbtn_down);
    BUTTON_DISPATCH(enumAdcNavKeyUp, on_upbtn_up, on_upbtn_down);
    BUTTON_DISPATCH(enumAdcNavKeyLeft, on_leftbtn_up, on_leftbtn_down);
    BUTTON_DISPATCH(enumAdcNavKeyRight, on_rightbtn_up, on_rightbtn_down);
}
