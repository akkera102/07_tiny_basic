#ifndef OLED_H
#define OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------
#define OLED_FONT_CX			5
#define OLED_FONT_CY			8
#define OLED_FONT_SIZE			5

#define OLED_SCREEN_CX			128
#define OLED_SCREEN_CY			64
#define OLED_SCREEN_FONT_CX		(OLED_SCREEN_CX / OLED_FONT_CX)
#define OLED_SCREEN_FONT_CY		(OLED_SCREEN_CY / OLED_FONT_CY)

//---------------------------------------------------------------------------
#if defined(ARDUBOY_10)

	#define OLED_PIN_CS			12
	#define OLED_PIN_DC			4
	#define OLED_PIN_RST		6

#elif defined(ARDUBOY_DEVKIT)

	#define OLED_PIN_CS			6
	#define OLED_PIN_DC			4
	#define OLED_PIN_RST		12

#endif

//---------------------------------------------------------------------------
typedef struct {
	u8 buf[OLED_SCREEN_CX * (OLED_SCREEN_CY / 8)];

} ST_OLED;


//---------------------------------------------------------------------------
void OledInit(void);
void OledDisplay(void);

void OledDrawStr(u8 fx, u8 fy, const char* fmt, ...);
void OledDrawStrP(u8 fx, u8 fy, const char* s);
void OledDrawChr(u8 fx, u8 fy, char chr);
void OledDrawDot(u8 x, u8 y);
void OledDrawCls(void);

void OledDrawBmp(s8 sx, s8 sy, u8* p);
void OledDrawPng8(s8 bx, s8 by, u8* p, u8 num);


#ifdef __cplusplus
}
#endif
#endif
