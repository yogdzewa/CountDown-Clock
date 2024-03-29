#include "callback.h"
#define CALLBACK_CNT 6
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
    {enumEventUart1Rxd, on_uart1_rx},
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
    u_byte key_state;
    BUTTON_DISPATCH(enumKey1, on_btn1_up, on_btn1_down);
    BUTTON_DISPATCH(enumKey2, on_btn2_up, on_btn2_down);
}

void __on_nav()
{
    u_byte key_state;
    NAVADC_DISPATCH(enumAdcNavKey3, on_btn3_down);
    NAVADC_DISPATCH(enumAdcNavKeyCenter, on_nav_down);
    NAVADC_DISPATCH_WITH_UP(enumAdcNavKeyDown, on_downbtn_up, on_downbtn_down);
    NAVADC_DISPATCH(enumAdcNavKeyUp, on_upbtn_down);
    NAVADC_DISPATCH(enumAdcNavKeyLeft, on_leftbtn_down);
    NAVADC_DISPATCH(enumAdcNavKeyRight, on_rightbtn_down);
}
