#ifndef FRAME_H
#define FRAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------
typedef struct {
	u8   rate;
	u16  count;
	u32  nextStart;
	u32  lastStart;
	u8   lastDurationMs;
	u8   eachMillis;
	bool isPostRender;

	u16  cpuMaxPercent;
	u8   cnt;
} ST_FRAME;


//---------------------------------------------------------------------------
void FrameInit(void);
void FrameSetRate(u8 rate);
u16  FrameGetCpuPercent(void);
u16  FrameGetCpuPercentMax(void);
u8   FrameGetCnt(void);
bool FrameIsMod(u8 num);

bool FrameLoop(void);


#ifdef __cplusplus
}
#endif
#endif
