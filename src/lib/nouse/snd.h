#ifndef SND_H
#define SND_H
#ifdef __cplusplus
extern "C" {
#endif

// ISR(TIMER3_COMPA_vect)
// ISR(TIMER1_COMPA_vect)

#include "common.h"

//---------------------------------------------------------------------------
#define SND_MAX_CH				2

enum {
	SND_OP_PLAY_NOTE = 0x90,
	SND_OP_STOP_NOTE = 0x80,
	SND_OP_RESTART   = 0xe0,
	SND_OP_STOP      = 0xf0,
};

//---------------------------------------------------------------------------
#if defined(ARDUBOY_10)

	#define SND_PIN1 5			// PC6
	#define SND_PIN2 13			// PC7

#elif defined(ARDUBOY_DEVKIT)

	#define SND_PIN1 A2
	#define SND_PIN2 A3

#endif

//---------------------------------------------------------------------------

typedef struct {
	vu8* pPort;
	u8   mask;
} ST_SND_PIN;

typedef struct {
	bool isPlay;
	u32  cnt;
} ST_SND_TONE;


typedef struct {

	ST_SND_TONE t[SND_MAX_CH];
	ST_SND_PIN  p[SND_MAX_CH];

} ST_SND;


//---------------------------------------------------------------------------
void SndInit(void);

void SndPlay(u8 ch, u16 freq, u16 duration);
void SndStartTimer(u8 ch, u32 freq);
void SndStopTimer(u8 ch);


#ifdef __cplusplus
}
#endif
#endif
