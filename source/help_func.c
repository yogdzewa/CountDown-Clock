#include "help_func.h"
#include "callback.h"
// used for reset value
XDATA uchar TIME_RELD_H;
XDATA uchar TIME_RELD_M;
XDATA uchar TIME_RELD_S;
XDATA uchar TIME_REST_M;
// used for countdown process
// uchar TIME_LIMIT_hour = 0;
// uchar TIME_LIMIT_minute = 50;
// uchar TIME_LIMIT_second = 21;
int TIME_LIMIT_ALLSEC;
// tmp var and macros
int diff;
int diff_t;
// #define CLOCK_SAME_COND(ele) (base.##ele == cur.##ele)
// #define CLKs_DIFF(ele) ((int)cur.##ele - (int)base.##ele)
// #define CLK_LIMIT_DIFF(ele) (TIME_LIMIT_##ele - time_diff_tmp.##ele)
// #define CNT_DOWN(ele, prv) ((CLK_LIMIT_DIFF(ele) < (uchar)60) ? CLK_LIMIT_DIFF(ele) : (time_diff_tmp.##prv--, ((uchar)60 + CLK_LIMIT_DIFF(ele))))

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