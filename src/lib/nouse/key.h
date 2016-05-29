#ifndef KEY_H
#define KEY_H
#ifdef __cplusplus
extern "C" {
#endif

#include <pins_arduino.h>
#include "common.h"

//---------------------------------------------------------------------------
#define KEY_REPEAT_CNT				6


//---------------------------------------------------------------------------
// Using symbolic pin names from pins_arduino.h
#if defined(ARDUBOY_10)

	#define KEY_PIN_L				A2		// PF5
	#define KEY_PIN_R				A1		// PF6
	#define KEY_PIN_U				A0		// PF7
	#define KEY_PIN_D				A3		// PF4
	#define KEY_PIN_A				7		// PE6
	#define KEY_PIN_B				8		// PB4

#elif defined(ARDUBOY_DEVKIT)

	#define KEY_PIN_L				9
	#define KEY_PIN_R				5
	#define KEY_PIN_U				8
	#define KEY_PIN_D				10
	#define KEY_PIN_A				A0
	#define KEY_PIN_B				A1

#endif

//---------------------------------------------------------------------------
enum {

#if defined(ARDUBOY_10)

	KEY_A = _BV(3),
	KEY_B = _BV(2),
	KEY_L = _BV(5),
	KEY_R = _BV(6),
	KEY_U = _BV(7),
	KEY_D = _BV(4),

#elif defined(ARDUBOY_DEVKIT)

	KEY_A = _BV(0),
	KEY_B = _BV(1),
	KEY_R = _BV(2),
	KEY_U = _BV(4),
	KEY_L = _BV(5),
	KEY_D = _BV(6),

#endif

	KEY_ALL = (KEY_U | KEY_D | KEY_L | KEY_R | KEY_A | KEY_B),
};

//---------------------------------------------------------------------------
typedef struct {
	u8 cnt;
	u8 trg;
	u8 off;
	u8 rep;
	s8 repCnt;

} ST_KEY;

//---------------------------------------------------------------------------
void KeyInit(void);
void KeyLoop(void);

u8   KeyGetCnt(void);
u8   KeyGetTrg(void);
u8   KeyGetOff(void);
u8   KeyGetRep(void);


#ifdef __cplusplus
}
#endif
#endif
