#include "help_func.h"
#include "callback.h"
// used for reset value
XDATA uchar TIME_RELD_H = 0;
XDATA uchar TIME_RELD_M = 50;
XDATA uchar TIME_RELD_S = 21;
XDATA uchar TIME_REST_M = 11;
// used for countdown process
uchar TIME_LIMIT_hour = 0;
uchar TIME_LIMIT_minute = 50;
uchar TIME_LIMIT_second = 21;
// tmp var and macros
int diff;
int diff_t;
#define CLOCK_SAME_COND(ele) (base.##ele == cur.##ele)
#define CLKs_DIFF(ele) ((int)cur.##ele - (int)base.##ele)
#define CLK_LIMIT_DIFF(ele) (TIME_LIMIT_##ele - time_diff_tmp.##ele)
#define CNT_DOWN(ele, prv) ((CLK_LIMIT_DIFF(ele) < (uchar)60) ? CLK_LIMIT_DIFF(ele) : (time_diff_tmp.##prv--, ((uchar)60 + CLK_LIMIT_DIFF(ele))))

void time_diff_count_down(struct_DS1302_RTC base, struct_DS1302_RTC cur)
{
    diff = CLKs_DIFF(hour) * 3600 + CLKs_DIFF(minute) * 60 + CLKs_DIFF(second);
    diff_t = (int)TIME_LIMIT_hour * 3600 + (int)TIME_LIMIT_minute * 60 + (int)TIME_LIMIT_second - diff;
    if (diff_t < 0)
    {
        diff_t = diff + diff_t + diff;
        TIME_LIMIT_hour = diff_t / 3600;
        TIME_LIMIT_minute = diff_t % 3600 / 60;
        TIME_LIMIT_second = diff_t % 60;
        diff_t = diff_t - diff;
    }
    time_diff_tmp.hour = diff_t / 3600;
    time_diff_tmp.minute = diff_t % 3600 / 60;
    time_diff_tmp.second = diff_t % 60;

    if (!CLOCK_SAME_COND(day))
        time_diff_tmp.hour = TIME_LIMIT_hour - ((23 - base.hour) + cur.hour);
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