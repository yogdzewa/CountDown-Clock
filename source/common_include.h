#ifndef _COMMON_
#define _COMMON_

//for convenience definition
typedef unsigned char u_byte;
typedef unsigned int uint;
typedef unsigned char uchar;
#define XDATA xdata

// for vscode auto complete
// #define autocomplete

#ifdef autocomplete
    #define pdata
    #define xdata
    #define bit int
#endif

//for beep mode
#define BEEP_MUTE ((char)0)



// Processor support
#include "inc/STC15F2K60S2.H"
#include <intrins.h>
#include <string.h>

// Message support
#include "inc/sys.h"

// Hardware and Functional support
#include "inc/ADC.h"
#include "inc/beep.h"
#include "inc/displayer.h"
#include "inc/DS1302.h"
#include "inc/EXT.h"
#include "inc/FM_Radio.h"
#include "inc/hall.H"
#include "inc/IR.h"
#include "inc/Key.H"
#include "inc/M24C02.h"
#include "inc/music.h"
#include "inc/StepMotor.h"
#include "inc/uart1.h"
#include "inc/uart2.h"
#include "inc/Vib.h"

#endif //_COMMON_