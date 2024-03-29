#include "main_loop.h"
#include "callback.h"
#include "help_func.h"
#include "display_decode.h"
uint led_acc = 0;
uchar led_seq_flag;
// for timeout event
void LED_rotate_disp()
{
    led_acc++;
    LedPrint(time_out_flag & startup_flag ? led_pos : 0);
    delay_ms(1);
    if (led_acc >= 20)
    {
        if (led_pos == 0x80 || led_pos == 0x01)
            led_seq_flag = led_seq_flag ^ 0x01;
        if (led_seq_flag)
            led_pos = _crol_(led_pos, 1);
        else
            led_pos = _cror_(led_pos, 1);
        led_acc = 0;
    }
}

#define dec_l(n) light##_array[n]
#define dec_t(n) timer##_array[n]
// for segment display
void SEG_disp()
{
    if (!startup_flag)
    {
        Seg7Print(20, 20, 20, 20, 20, 20, 20, 20);
        return;
    }
    if (seg_rop_flag == 1)
    {
        Seg7Print(dec_l(2), dec_l(1), dec_l(0), 20, 20, dec_l(5), dec_l(4), dec_l(3));
        return;
    }
    if (work_time_adjust_flag)
    {
        Seg7Print(
            TIME_WORK_H / 10, TIME_WORK_H % 10, 21,
            TIME_WORK_M / 10, TIME_WORK_M % 10, 21,
            TIME_WORK_S / 10, TIME_WORK_S % 10);
        return;
    }
    if (rest_time_adjust_flag)
    {
        Seg7Print(
            20, 20, 21,
            TIME_REST_M / 10, TIME_REST_M % 10, 21,
            0, 0);
        return;
    }
    #define LIGHT_OFF light_sensor_flag * 10
    #define PC_OFF pc_connect_flag * 10
    // for timer display
    if (!rest_flag)
        Seg7Print(dec_t(5), dec_t(4), 20, dec_t(3), dec_t(2) + PC_OFF, 20, dec_t(1), dec_t(0) + LIGHT_OFF);
    else
        Seg7Print(20, 20, 20, dec_t(3), dec_t(2) + PC_OFF, 20, dec_t(1), dec_t(0) + LIGHT_OFF);
}