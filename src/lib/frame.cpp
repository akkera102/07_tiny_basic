#include "frame.h"
#include <Arduino.h>
#include "sys.h"

// timer 0 millis()


//---------------------------------------------------------------------------
ST_FRAME Frame;


//---------------------------------------------------------------------------
void FrameInit(void)
{
	_Memset(&Frame, 0x00, sizeof(ST_FRAME));

	FrameSetRate(60);
}
//---------------------------------------------------------------------------
void FrameSetRate(u8 rate)
{
	Frame.rate = rate;
	Frame.eachMillis = 1000 / rate;
}
//---------------------------------------------------------------------------
u16 FrameGetCpuPercent(void)
{
	return (Frame.lastDurationMs * 100) / Frame.eachMillis;
}
//---------------------------------------------------------------------------
u16 FrameGetCpuPercentMax(void)
{
	u16 per = FrameGetCpuPercent();

	if(Frame.cpuMaxPercent < per)
	{
		Frame.cpuMaxPercent = per;
	}

	return Frame.cpuMaxPercent;
}
//---------------------------------------------------------------------------
u8 FrameGetCnt(void)
{
	return Frame.cnt;
}
//---------------------------------------------------------------------------
bool FrameIsMod(u8 num)
{
	return (Frame.cnt % num == 0) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool FrameLoop(void)
{
	u32 now = millis();

	if(Frame.isPostRender == TRUE)
	{
		Frame.lastDurationMs = now - Frame.lastStart;
		Frame.count++;

		Frame.isPostRender = FALSE;
	}

	if(now < Frame.nextStart)
	{
		u8 remaining = Frame.nextStart - now;

		if(remaining > 1)
		{
			SysIdle();
		}

		return FALSE;
	}

	Frame.nextStart = Frame.lastStart + Frame.eachMillis;

	if(Frame.nextStart < now)
	{
		Frame.nextStart = now;
	}

	Frame.lastStart = now;
	Frame.isPostRender = TRUE;
	Frame.cnt++;

	return TRUE;
}
