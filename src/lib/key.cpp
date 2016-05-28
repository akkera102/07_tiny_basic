#include "key.h"
#include <Arduino.h>

//---------------------------------------------------------------------------
ST_KEY Key;


//---------------------------------------------------------------------------
void KeyInit(void)
{
	_Memset(&Key, 0x00, sizeof(ST_KEY));

	pinMode(KEY_PIN_L, INPUT_PULLUP);
	pinMode(KEY_PIN_R, INPUT_PULLUP);
	pinMode(KEY_PIN_U, INPUT_PULLUP);
	pinMode(KEY_PIN_D, INPUT_PULLUP);
	pinMode(KEY_PIN_A, INPUT_PULLUP);
	pinMode(KEY_PIN_B, INPUT_PULLUP);
}
//---------------------------------------------------------------------------
void KeyLoop(void)
{
	u8 cnt;

#if defined(ARDUBOY_10)

	cnt  = ((~PINF) & 0xf0);			// up, down, left, right
	cnt |= ((~PINE) & 0x40) >> 3;		// A
	cnt |= ((~PINB) & 0x10) >> 2;		// B

#elif defined(ARDUBOY_DEVKIT)

	cnt  = ((~PINB) & 0x70);			// down, left, up
	cnt |= ((~PINC) & 0x40) >> 4;		// right
	cnt |= ((~PINF) & 0xc0) >> 6;		// A, B

#endif

	Key.trg = (Key.trg ^ cnt) & ~Key.cnt;
	Key.off = ~cnt & Key.cnt;
	Key.cnt = cnt;


	if(Key.trg & KEY_ALL || Key.repCnt == 0)
	{
		Key.rep = Key.cnt;
		Key.repCnt = KEY_REPEAT_CNT;
	}
	else
	{
		Key.rep = 0;
	}

	if(Key.cnt & KEY_ALL)
	{
		if(Key.repCnt != 0) Key.repCnt--;
	}
	else
	{
		Key.repCnt = 0;
	}
}
//---------------------------------------------------------------------------
u8 KeyGetCnt(void)
{
	return Key.cnt;
}
//---------------------------------------------------------------------------
u8 KeyGetTrg(void)
{
	return Key.trg;
}
//---------------------------------------------------------------------------
u8 KeyGetOff(void)
{
	return Key.off;
}
//---------------------------------------------------------------------------
u8 KeyGetRep(void)
{
	return Key.rep;
}
