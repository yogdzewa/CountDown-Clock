#include "common_include.h"

#include "init.h"
#include "display_decode.h"
#include "callback.h"
// #include "messageloop.h"
#include "state_control.h"
#include "main_loop.h"

void main()
{
	global_init();
	init_callbacks();

	MySTC_Init();
	//init light base and clock base.
	clock_base = RTC_Read();
	light_base = adc_res.Rop;
	while (1)
	{
		MySTC_OS();
		LED_rotate_disp();
		SEG_disp();
	}
	// message_loop();
}
