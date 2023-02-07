#include "common_include.h"
#include "help_func.h"
#include "display_decode.h"
#include "callback.h"
#include "init.h"
uchar led_pos = 1;
pdata struct_DS1302_RTC clock_cur;
uint clock_base_totalsec;
uint clock_cur_totalsec;
bit set_time_flag;
pdata struct_ADC adc_res;
uchar adc_cnt;
uint adc_acc;
pdata uint light_base = 0;
pdata uint light_cur;
uchar light_acc;
bit time_out_flag; // time out led rotate indicator
bit time_stop_flag;
bit seg_rop_flag;     // seg indicator flag
uchar light_array[6]; // seg display register
uchar timer_array[6]; // seg dispaly register
struct_time_diff time_diff_tmp;
bit vibra_flag;
bit work_time_adjust_flag;
bit rest_flag;
bit rest_time_adjust_flag;
bit startup_flag = 1;
bit light_sensor_flag = 1;
bit pc_connect_flag;
bit beep_mute_flag;
bit auto_switch_flag;
XDATA uchar recvinfo[10] = { 0 };
// used for reset value
XDATA uchar TIME_WORK_H;
XDATA uchar TIME_WORK_M;
XDATA uchar TIME_WORK_S;
XDATA uchar TIME_REST_M;
// reset the timer start
void on_btn1_down()
{
    DS1302Init(time);
    clock_read_2sec(CLOCK_BASE);
    if (!rest_flag)
    {
        TIME_LIMIT_ALLSEC = TIME_WORK_H * 3600;
        TIME_LIMIT_ALLSEC += TIME_WORK_M * 60;
        TIME_LIMIT_ALLSEC += TIME_WORK_S;
        Uart1Print("NOML:", 5);
    }
    else
    {
        TIME_LIMIT_ALLSEC = TIME_REST_M * 60;
        TIME_LIMIT_ALLSEC += 5;
        Uart1Print("REST:", 5);
    }
}
void on_btn2_down() {
    light_sensor_flag = ~light_sensor_flag;
    if (light_sensor_flag) {
        light_acc = 0;
        light_base = adc_res.Rop;
        light_array[2] = light_base / 100;
        light_array[1] = light_base % 100 / 10;
        light_array[0] = light_base % 10;
        seg_rop_flag = 1;
    }
}
void on_btn2_up() { seg_rop_flag = 0; }
void on_btn3_down()
{ // adjust time interval
    work_time_adjust_flag = 1;
    rest_time_adjust_flag = 0;
}
void on_nav_down()
{
    if (work_time_adjust_flag || rest_time_adjust_flag)
    {
        work_time_adjust_flag = rest_time_adjust_flag = 0;
        // store TIME_WORK** into M24C02
        // H -> 0x00, M -> 0x01, S -> 0x02, REST_M -> 0x03
        // the time interval is critical, so use delay function.
        M24C02_Write(0, TIME_WORK_H);
        delay_ms(10);
        M24C02_Write(1, TIME_WORK_M);
        delay_ms(10);
        M24C02_Write(2, TIME_WORK_S);
        delay_ms(10);
        M24C02_Write(3, TIME_REST_M);
        on_btn1_down();
    }
    else
    {
        if ((startup_flag = ~startup_flag) == 1)
            rest_flag = light_acc = 0, light_base = adc_res.Rop, on_btn1_down();
        else
            Uart1Print("STOP:", 5);
    }
}
#define TIME_INTERVAL 5
#define RTIME_INTERVAL 2
#define TIME_ADD (TIME_WORK_M + TIME_INTERVAL)
#define TIME_ABSTRACT (TIME_WORK_M - TIME_INTERVAL)
#define RTIME_ABSTRACT (TIME_REST_M - RTIME_INTERVAL)
#define RTIME_ADD (TIME_REST_M + RTIME_INTERVAL)
void on_downbtn_down()
{
    if (work_time_adjust_flag)
    {
        TIME_WORK_M = (TIME_ABSTRACT < (uchar)60) ? TIME_ABSTRACT : (TIME_WORK_H--, ((uchar)60 + TIME_ABSTRACT));
        TIME_WORK_H = TIME_WORK_H % 55;
    }
    else if (rest_time_adjust_flag)
        TIME_REST_M = (RTIME_ABSTRACT < (uchar)60) ? RTIME_ABSTRACT : ((uchar)60 + RTIME_ABSTRACT);
    else
    {
        M24C02_Write(4, (beep_mute_flag = ~beep_mute_flag));
        beep_mute_flag ? SetBeep(30, 2) : SetBeep(5000, 2);
    }
}
void on_downbtn_up() { return; }
void on_upbtn_down()
{
    if (work_time_adjust_flag)
    {
        TIME_WORK_M = (TIME_ADD < (uchar)60) ? TIME_ADD : (TIME_WORK_H++, (TIME_ADD - (uchar)60));
        TIME_WORK_H = TIME_WORK_H % 55;
    }
    else if (rest_time_adjust_flag)
        TIME_REST_M = (RTIME_ADD < (uchar)60) ? RTIME_ADD : (RTIME_ADD - (uchar)60);
    else // if (!light_sensor_flag)
    {
        time_out_flag = 0;
        if (!rest_flag)
            TIME_LIMIT_ALLSEC += 5 * 60;
        else
            TIME_LIMIT_ALLSEC += 2 * 60;
    }
}
void on_leftbtn_down()
{
    if (rest_time_adjust_flag | work_time_adjust_flag)
    {
        rest_time_adjust_flag = work_time_adjust_flag;
        work_time_adjust_flag = ~work_time_adjust_flag;
    }
    else {
        M24C02_Write(5, (auto_switch_flag = ~auto_switch_flag));
        auto_switch_flag ? SetBeep(5000, 2) : SetBeep(30, 2);
    }
}
void on_rightbtn_down()
{
    if (rest_time_adjust_flag | work_time_adjust_flag)
    {
        rest_time_adjust_flag = work_time_adjust_flag;
        work_time_adjust_flag = ~work_time_adjust_flag;
    }
    else
        rest_flag = ~rest_flag, on_btn1_down();
}

#undef TIME_ADD
#undef RTIME_ADD
#undef RTIME_ABSTRACT
#undef TIME_ABSTRACT
void on_sensor_vib()
{
    char interval = rest_flag ? 2 : 5;
    SetBeep(30, 1);
    if (TIME_LIMIT_ALLSEC - interval * 60 >= 0)
        TIME_LIMIT_ALLSEC -= interval * 60;
    else
        TIME_LIMIT_ALLSEC %= 60;
}

// read real time clock, and calc the diff time.
void on_timer_100ms()
{
    if (!startup_flag)
    {
        if (adc_res.Rop >= 27 && auto_switch_flag)
            startup_flag = 1, rest_flag = light_acc = 0, light_base = adc_res.Rop, on_btn1_down();
        else
            return;
    }
    if (!time_stop_flag)
    {
        clock_read_2sec(CLOCK_CUR);
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
                    beep_mute_flag ? SetBeep(30, 2) : SetBeep(5000, 20);
                }
                else
                {
                    Uart1Print("NTOU:", 5); // normal TIMEOUT
                    beep_mute_flag ? BEEP_MUTE : SetBeep(5000, 1);
                }
            }
        }
    }
    // judge Rop diff
    if (light_sensor_flag)
        if (((int)light_base - (int)light_cur > 10))
        {
            if (++light_acc > 14)
                pc_connect_flag = startup_flag = 0;
        }
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
    if (!light_sensor_flag)
        return;
    if (light_base == 0 && first <= 15)
        light_base = adc_res.Rop, first++;
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
bit tlig = 0;
void on_uart1_rx()
{
    if (!strncmp(recvinfo + 2, "CONN", 4))
    {
        // recv CONN when already connected or switch off, reset to normal mode
        // and toggle switch
        if (pc_connect_flag || !startup_flag)
        {
            if (!(pc_connect_flag && startup_flag)) // detect toggled swtich
                rest_flag = light_acc = 0, light_base = adc_res.Rop, on_btn1_down();
            startup_flag = ~startup_flag;
        }
        pc_connect_flag = 1;
    } // reset to rest countdown
    else if (!strncmp(recvinfo + 2, "RRST", 4))
        rest_flag = ~rest_flag, SetBeep(30, 2), on_btn1_down();
    else if (!strncmp(recvinfo + 2, "RSET", 4))
        on_btn1_down();
    // beep for a relative long time.
    else if (!strncmp(recvinfo + 2, "BEEP", 4))
        time_out_flag ? (beep_mute_flag ? SetBeep(30, 2) : SetBeep(400, 100)) : BEEP_MUTE;
    else if (!strncmp(recvinfo + 2, "TLIG", 4))
    {
        if (tlig)
            tlig = ~tlig;
        else
        {
            light_sensor_flag = !(light_sensor_flag ^ seg_rop_flag);
            if (seg_rop_flag = !seg_rop_flag & light_sensor_flag)
                on_btn2_down();
            tlig = ~(light_sensor_flag | seg_rop_flag);
        }
    }
    else if (!strncmp(recvinfo + 2, "DISC", 4))
        pc_connect_flag = 0;
    else if (!strncmp(recvinfo + 2, "STOP", 4))
        pc_connect_flag = startup_flag = 0;
}

// void on_btn3_up() {}
// void on_timer_1ms() {}
// void on_timer_10ms() {}
// void on_timer_1sec() {}
void on_btn1_up() {}
// void on_nav_up() {}
// void on_upbtn_up() {}
// void on_leftbtn_up() {}
// void on_rightbtn_up() {}
// void on_ir_rx() {}
// void on_uart2_rx() {}
// void on_sensor_hall() {}