#include "common_include.h"
#include "help_func.h"
#include "display_decode.h"
#include "callback.h"
//test for local branch merge
// uint time_cur = 0;
uchar led_pos = 1;
pdata struct_DS1302_RTC clock_base;
pdata struct_DS1302_RTC clock_cur;
uint clock_base_totalsec;
uint clock_cur_totalsec;
bit set_time_flag;
pdata struct_ADC adc_res;
uchar adc_cnt;
uint adc_acc;
pdata uint light_base;
pdata uint light_cur;
uchar light_acc;
bit time_out_flag; // time out led rotate indicator
bit time_stop_flag;
bit seg_rop_flag;     // seg indicator flag
uchar light_array[6]; // seg display register
uchar timer_array[6]; // seg dispaly register
struct_time_diff time_diff_tmp;
bit vibra_flag;
bit seg_time_adjust_flag;
bit rest_flag;
bit rest_time_adjust_flag;
bit startup_flag = 1;
pdata uchar nvm_write_cnt = 3;
XDATA uchar info[5] = {0xff, 0x01, 0x10, 0xf0, 0x0f};
XDATA uchar recvinfo[5] = {0};
// reset the timer start
#undef pdata
void on_btn1_down()
{
    clock_base = RTC_Read();
    time_out_flag = 0;
    time_stop_flag = 0;
    clock_base_totalsec = ((clock_base.second & 0x70) >> 4) * 10 + (clock_base.second & 0x0F);
    clock_base_totalsec += (((clock_base.minute & 0x70) >> 4) * 10 + (clock_base.minute & 0x0F)) * 60;
    clock_base_totalsec += (((clock_base.hour & 0x70) >> 4) * 10 + (clock_base.hour & 0x0F)) * 3600;
    if (!rest_flag)
    {
        TIME_LIMIT_ALLSEC = TIME_RELD_H * 3600;
        TIME_LIMIT_ALLSEC += TIME_RELD_M * 60;
        TIME_LIMIT_ALLSEC += TIME_RELD_S;
        Uart1Print("NOML:", 5);
    }
    else
    {
        TIME_LIMIT_ALLSEC = TIME_REST_M * 60;
        TIME_LIMIT_ALLSEC += 5;
        Uart1Print("REST:", 5);
    }
}
#define dec(s, n) s##_array[n]
void on_btn2_down()
{ // set Rop light base
    light_base = adc_res.Rop;
    light_array[2] = light_base / 100;
    light_array[1] = light_base % 100 / 10;
    light_array[0] = light_base % 10;
    seg_rop_flag = 1;

    // for test uart
    // content: 0xa0 -> lockscreen
    Uart1Print(info, (uint)4);
}
void on_btn2_up() { seg_rop_flag = 0; }
void on_btn3_down()
{ // adjust time interval
    seg_time_adjust_flag = 1;
    rest_time_adjust_flag = 0;
}
void on_nav_down()
{
    if (seg_time_adjust_flag || rest_time_adjust_flag)
    {
        seg_time_adjust_flag = rest_time_adjust_flag = 0;
        // store TIME_RELD_** into M24C02
        // H -> 0x00, M -> 0x01, S -> 0x02, REST_M -> 0x03
        // the time interval is critical, so set the flag here
        //(for time callback function to countdown)
        M24C02_Write(0, TIME_RELD_H);
        delay_ms(10);
        M24C02_Write(1, TIME_RELD_M);
        delay_ms(10);
        M24C02_Write(2, TIME_RELD_S);
        delay_ms(10);
        M24C02_Write(3, TIME_REST_M);
        on_btn1_down();
    }
    else
    {
        if ((startup_flag = ~startup_flag) == 1)
            rest_flag = 0, on_btn1_down();
        else
            Uart1Print("STOP:", 5);
    }
}
#define TIME_INTERVAL 5
#define RTIME_INTERVAL 2
#define TIME_ADD (TIME_RELD_M + TIME_INTERVAL)
#define TIME_ABSTRACT (TIME_RELD_M - TIME_INTERVAL)
#define RTIME_ABSTRACT (TIME_REST_M - RTIME_INTERVAL)
#define RTIME_ADD (TIME_REST_M + RTIME_INTERVAL)
void on_downbtn_down()
{
    if (seg_time_adjust_flag)
    {
        TIME_RELD_M = (TIME_ABSTRACT < (uchar)60) ? TIME_ABSTRACT : (TIME_RELD_H--, ((uchar)60 + TIME_ABSTRACT));
        TIME_RELD_H = TIME_RELD_H % 55;
    }
    else if (rest_time_adjust_flag)
        TIME_REST_M = (RTIME_ABSTRACT < (uchar)60) ? RTIME_ABSTRACT : ((uchar)60 + RTIME_ABSTRACT);
}
void on_upbtn_down()
{
    if (seg_time_adjust_flag)
    {
        TIME_RELD_M = (TIME_ADD < (uchar)60) ? TIME_ADD : (TIME_RELD_H++, (TIME_ADD - (uchar)60));
        TIME_RELD_H = TIME_RELD_H % 55;
    }
    else if (rest_time_adjust_flag)
        TIME_REST_M = (RTIME_ADD < (uchar)60) ? RTIME_ADD : (RTIME_ADD - (uchar)60);
}
void on_leftbtn_down()
{
    if (rest_time_adjust_flag | seg_time_adjust_flag)
    {
        rest_time_adjust_flag = seg_time_adjust_flag;
        seg_time_adjust_flag = ~seg_time_adjust_flag;
    }
}
void on_rightbtn_down()
{
    // SetBeep(8000, 5);
    if (rest_time_adjust_flag | seg_time_adjust_flag)
    {
        rest_time_adjust_flag = seg_time_adjust_flag;
        seg_time_adjust_flag = ~seg_time_adjust_flag;
    }
}

#undef TIME_ADD
#undef RTIME_ADD
#undef RTIME_ABSTRACT
#undef TIME_ABSTRACT
void on_sensor_vib()
{
    SetBeep(30, 1);
    if (!rest_flag)
        if (TIME_LIMIT_ALLSEC - 5 * 60 >= 0)
            TIME_LIMIT_ALLSEC -= 5 * 60;
        else
            TIME_LIMIT_ALLSEC %= 60;
    else
    {
        if (TIME_LIMIT_ALLSEC - 2 * 60 >= 0)
            TIME_LIMIT_ALLSEC -= 2 * 60;
        else
            TIME_LIMIT_ALLSEC %= 60;
    }
}
// read real time clock, and reverse the led(for test function)
void on_timer_100ms()
{
    if (!startup_flag)
        return;
    if (!time_stop_flag)
    {
        clock_cur = RTC_Read();
        clock_cur_totalsec = ((clock_cur.second & 0x70) >> 4) * 10 + (clock_cur.second & 0x0F);
        clock_cur_totalsec += (((clock_cur.minute & 0x70) >> 4) * 10 + (clock_cur.minute & 0x0F)) * 60;
        clock_cur_totalsec += (((clock_cur.hour & 0x70) >> 4) * 10 + (clock_cur.hour & 0x0F)) * 3600;
        // following function modifies @time_diff_tmp variable
        time_diff_count_down();
        timer_array[1] = time_diff_tmp.second / 10;
        timer_array[0] = time_diff_tmp.second % 10;

        timer_array[3] = time_diff_tmp.minute / 10;
        timer_array[2] = time_diff_tmp.minute % 10;

        timer_array[5] = time_diff_tmp.hour / 10;
        timer_array[4] = time_diff_tmp.hour % 10;
        if (time_diff_tmp.minute == 0 && time_diff_tmp.hour == 0 && time_diff_tmp.second <= 20)
        {
            time_out_flag = 1;
            if (time_diff_tmp.second == 0)
            {
                time_stop_flag = 1;
                if (rest_flag)
                {
                    Uart1Print("RTOU:", 5); // rest TIMEOUT
                    SetBeep(5000, 20);
                }
                else
                {
                    Uart1Print("NTOU:", 5); // normal TIMEOUT
                    SetBeep(5000, 1);
                }
            }
        }
    }
    // judge rop diff
    if (((int)light_base - (int)light_cur > 10))
        light_acc++;
    else
    {
        if (light_acc >= 2 && light_acc <= 6)
        {
            time_out_flag = 0;
            if (!rest_flag)
                TIME_LIMIT_ALLSEC += 5 * 60;
            else
                TIME_LIMIT_ALLSEC += 2 * 60;
        }
        else if (light_acc >= 7)
            rest_flag = ~rest_flag, on_btn1_down();
        light_acc = 0;
    }
}
void on_event_adc()
{
    adc_res = GetADC();
    adc_cnt++;
    if (adc_cnt == 30)
    {
        light_cur = (adc_acc + 15) / 30;
        light_array[5] = light_cur / 100;
        light_array[4] = light_cur % 100 / 10;
        light_array[3] = light_cur % 10;
        adc_cnt = 0;
        adc_acc = 0;
    }
    else
        adc_acc += adc_res.Rop;
}
void on_uart1_rx()
{
    // reset to reset countdown
    if (!strncmp(recvinfo + 2, "RRST", 4))
        rest_flag = 1, SetBeep(5000, 5), on_btn1_down();
    else if (!strncmp(recvinfo + 2, "BEEP", 4))
        SetBeep(400, 100);
}

// void on_btn3_up() {}
// void on_timer_1ms() {}
// void on_timer_10ms() {}
// void on_timer_1sec() {}
void on_btn1_up() {}
// void on_nav_up() {}
// void on_downbtn_up() {}
// void on_upbtn_up() {}
// void on_leftbtn_up() {}
// void on_rightbtn_up() {}
// void on_ir_rx() {}
// void on_uart2_rx() {}
// void on_sensor_hall() {}