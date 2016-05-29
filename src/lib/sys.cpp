#include "sys.h"
#include <avr/power.h>
#include <avr/sleep.h>
#include "frame.h"
// #include "snd.h"
// #include "key.h"
#include "oled.h"
// #include "eep.h"
// #include "math.h"
#include "rnd.h"
// #include "debug.h"

//---------------------------------------------------------------------------
void SysInit(void)
{
	power_timer2_disable();
	power_adc_disable();
	power_twi_disable();
	power_usart0_disable();
	power_usart1_disable();

	OledInit();
	FrameInit();
//	SndInit();
//	EepInit();
//	KeyInit();
//	MathInit();
	RndInit();
//	DebugInit();
}
//---------------------------------------------------------------------------
void SysIdle(void)
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
}
//---------------------------------------------------------------------------
bool SysLoop(void)
{
	if(FrameLoop() == FALSE)
	{
		return FALSE;
	}

//	KeyLoop();
	return TRUE;
}
//---------------------------------------------------------------------------
void SysLoopEnd(void)
{
	OledDisplay();
//	OledDrawCls();
}
