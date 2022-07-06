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
        Seg7Print(36, 36, 36, 36, 36, 36, 36, 36);
        return;
    }
    if (seg_rop_flag == 1)
    {
        Seg7Print(dec_l(2), dec_l(1), dec_l(0), 36, 36, dec_l(5), dec_l(4), dec_l(3));
        return;
    }
    if (seg_time_adjust_flag)
    {
        Seg7Print(
            TIME_RELD_H / 10, TIME_RELD_H % 10, 37,
            TIME_RELD_M / 10, TIME_RELD_M % 10, 37,
            TIME_RELD_S / 10, TIME_RELD_S % 10);
        return;
    }
    if (rest_time_adjust_flag)
    {
        Seg7Print(
            36, 36, 37,
            TIME_REST_M / 10, TIME_REST_M % 10, 37,
            0, 0);
        return;
    }
    // for timer display
    if (!rest_flag)
        Seg7Print(dec_t(5), dec_t(4), 36, dec_t(3), dec_t(2), 36, dec_t(1), dec_t(0));
    else
        Seg7Print(36, 36, 36, dec_t(3), dec_t(2), 36, dec_t(1), dec_t(0));
}