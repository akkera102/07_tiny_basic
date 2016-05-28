#include "snd.h"
#include <Arduino.h>
#include <avr/power.h>
#include "oled.h"

//---------------------------------------------------------------------------
ST_SND Snd;


//---------------------------------------------------------------------------
void SndInit(void)
{
	_Memset(&Snd, 0x00, sizeof(ST_SND));

	pinMode(SND_PIN1, OUTPUT);
	Snd.p[0].pPort = portOutputRegister(digitalPinToPort(SND_PIN1));
	Snd.p[0].mask  = digitalPinToBitMask(SND_PIN1);

#if defined(ARDUBOY_10)

	pinMode(SND_PIN2, OUTPUT);
	Snd.p[1].pPort = portOutputRegister(digitalPinToPort(SND_PIN2));
	Snd.p[1].mask  = digitalPinToBitMask(SND_PIN2);

#endif

	TCCR3A = 0;
	TCCR3B = 0;
	TCCR1A = 0;
	TCCR1B = 0;

	bitWrite(TCCR3B, WGM32, 1);
	bitWrite(TCCR3B, CS30,  1);
	bitWrite(TCCR1B, WGM12, 1);
	bitWrite(TCCR1B, CS10,  1);

	power_timer3_enable();
	power_timer1_enable();
}
//---------------------------------------------------------------------------
void SndPlay(u8 ch, u16 freq, u16 duration)
{
	ST_SND_TONE* p = &Snd.t[ch];

	if(p->isPlay == TRUE)
	{
		return;
	}
	p->isPlay = TRUE;


	u32 cnt  = 2 * freq * duration / 1000;

	if(cnt == 0)
	{
		return;
	}
	p->cnt = cnt;


	SndStartTimer(ch, F_CPU / freq / 2);
}
//---------------------------------------------------------------------------
void SndStartTimer(u8 ch, u32 freq)
{
	// timer ck/1
	u32 ocr = freq;
	u8  pre = 0x01;

	if(ocr > 0xffff)
	{
		// ck/64
		ocr /= 64;
		pre  = 0x03;
	}
	ocr--;


	if(ch == 0)
	{
		TCCR3B = (TCCR3B & 0xf8) | pre;
		OCR3A  = ocr;
		bitWrite(TIMSK3, OCIE3A, 1);
	}
	else
	{
		TCCR1B = (TCCR1B & 0xf8) | pre;
		OCR1A  = ocr;
		bitWrite(TIMSK1, OCIE1A, 1);
	}
}
//---------------------------------------------------------------------------
void SndStopTimer(u8 ch)
{
	if(ch == 0)
	{
		TIMSK3 &= ~(1 << OCIE3A);
		*Snd.p[0].pPort &= ~Snd.p[0].mask;
	}
	else
	{
		TIMSK1 &= ~(1 << OCIE1A);

#if defined(ARDUBOY_10)

		*Snd.p[1].pPort &= ~Snd.p[1].mask;

#endif

	}

	Snd.t[ch].isPlay = FALSE;
}
//---------------------------------------------------------------------------
// TIMER 3 ch0
ISR(TIMER3_COMPA_vect)
{
	*Snd.p[0].pPort ^= Snd.p[0].mask;

	if(--Snd.t[0].cnt == 0)
	{
		SndStopTimer(0);
	}
}
//---------------------------------------------------------------------------
// TIMER 1 ch1
ISR(TIMER1_COMPA_vect)
{

#if defined(ARDUBOY_10)

	*Snd.p[1].pPort ^= Snd.p[1].mask;

#endif

	if(--Snd.t[1].cnt == 0)
	{
		SndStopTimer(1);
	}
}
