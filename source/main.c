#include "common_include.h"

#include "init.h"
#include "display_decode.h"
#include "callback.h"
#include "main_loop.h"
short first = 1;

void main()
{
	global_init();
	init_callbacks();

	MySTC_Init();
	// init light base and clock base.
	while (1)
	{
		MySTC_OS();
		if (first == 1)
			clock_read_2sec(CLOCK_BASE), first = 0;
		LED_rotate_disp();
		SEG_disp();
	}
	// message_loop();
}
