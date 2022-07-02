#include "messageloop.h"

void message_loop()
{
    MySTC_Init();
    while(1)
    {
        MySTC_OS();
    }
}