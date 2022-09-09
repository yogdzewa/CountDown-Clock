#include "help_func.h"
#include "callback.h"

// used for countdown process
int TIME_LIMIT_ALLSEC;
// tmp var
int diff;
int diff_t;

void time_diff_count_down()
{
    diff = clock_cur_totalsec - clock_base_totalsec;
    diff_t = TIME_LIMIT_ALLSEC - diff;
    if (diff_t < 0)
    {
        diff_t = (60 + diff_t) % 60;
        TIME_LIMIT_ALLSEC = diff_t + diff;
    }
    time_diff_tmp.hour = diff_t / 3600;
    time_diff_tmp.minute = diff_t % 3600 / 60;
    time_diff_tmp.second = diff_t % 60;
}

void clock_read_2sec(bit t)
{
    if (t == CLOCK_CUR)
    {
        clock_cur = RTC_Read();
        clock_cur_totalsec = ((clock_cur.second & 0x70) >> 4) * 10 + (clock_cur.second & 0x0F);
        clock_cur_totalsec += (((clock_cur.minute & 0x70) >> 4) * 10 + (clock_cur.minute & 0x0F)) * 60;
        clock_cur_totalsec += (((clock_cur.hour & 0x70) >> 4) * 10 + (clock_cur.hour & 0x0F)) * 3600;
    }
    else
    {
        clock_base = RTC_Read();
        time_out_flag = time_stop_flag = 0;
        clock_base_totalsec = ((clock_base.second & 0x70) >> 4) * 10 + (clock_base.second & 0x0F);
        clock_base_totalsec += (((clock_base.minute & 0x70) >> 4) * 10 + (clock_base.minute & 0x0F)) * 60;
        clock_base_totalsec += (((clock_base.hour & 0x70) >> 4) * 10 + (clock_base.hour & 0x0F)) * 3600;
    }
}

void delay_ms(uint n)
{
    while (n)
    {
        uchar i, j;
        i = 11;
        j = 190;
        do
        {
            while (--j)
                ;
        } while (--i);
        n--;
    }
}