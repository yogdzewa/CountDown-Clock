#ifndef _CALLBACK_
#define _CALLBACK_

#include "common_include.h"
#include "help_func.h"
typedef struct
{
    u_byte event;
    void *callback;
} callback_register;

extern code callback_register all_callbacks[];
extern uint time_cur;
extern uchar led_pos;
extern pdata struct_DS1302_RTC clock_base;
extern pdata struct_DS1302_RTC clock_cur;
extern bit set_time_flag;
extern uint clock_base_totalsec;
extern uint clock_cur_totalsec;
extern pdata struct_ADC adc_res;
extern uchar adc_cnt;
extern uint adc_acc;
extern pdata uint light_base;
extern pdata uint light_cur;
extern uchar light_acc;
extern bit time_out_flag; // time out led rotate indicator
extern bit time_stop_flag;
extern bit seg_rop_flag;     // seg indicator flag
extern uchar light_array[6]; // seg display register
extern uchar timer_array[6]; // seg dispaly register
extern struct_time_diff time_diff_tmp;
extern bit vibra_flag;
extern bit seg_time_adjust_flag;
extern bit rest_flag;
extern bit rest_time_adjust_flag;
extern bit startup_flag;

extern uchar TIME_LIMIT_hour;
extern uchar TIME_LIMIT_minute;
extern uchar TIME_LIMIT_second;
extern XDATA uchar TIME_RELD_H;
extern XDATA uchar TIME_RELD_M;
extern XDATA uchar TIME_RELD_S;
extern int TIME_LIMIT_ALLSEC;

void init_callbacks();

//================

#define BUTTON_DISPATCH(enumBtn, funcUp, funcDown) \
    if (GetAdcNavAct(enumBtn) == enumKeyPress)     \
        funcDown();
// else if (GetAdcNavAct(enumBtn) == enumKeyRelease)
//     funcUp();

void __on_button();
void __on_nav();

//=================

void on_btn1_down();
void on_btn1_up();
void on_btn2_down();
void on_btn2_up();
void on_btn3_down();
void on_btn3_up();
void on_nav_down();
void on_nav_up();
void on_downbtn_down();
void on_downbtn_up();
void on_upbtn_down();
void on_upbtn_up();
void on_leftbtn_down();
void on_leftbtn_up();
void on_rightbtn_down();
void on_rightbtn_up();
void on_timer_1ms();
void on_timer_10ms();
void on_timer_100ms();
void on_timer_1sec();
void on_sensor_hall();
void on_sensor_vib();
void on_ir_rx();
void on_uart1_rx();
void on_uart2_rx();
void on_event_adc();

#endif //_CALLBACK_