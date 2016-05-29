#include "oled.h"
#include <stdarg.h>
#include <Arduino.h>
#include <SPI.h>
#include "../res/font.h"

// SSD1306 OLED

//---------------------------------------------------------------------------
PROGMEM const u8 OledBootProgram[] = {
	0xD5,	// Set Display Clock Divisor 0xF0
	0xF0,
	0x8D,	// Charge Pump Setting 0x14
	0x14,
	0xA1,	// Set Segment Re-map (A0) | (b0001)
	0xC8,	// Set COM Output Scan Direction
	0x81,	// Set Contrast 0xCF
	0xCF,
	0xD9,	// Set Precharge 0xF1
	0xF1,
	0xAF,	// Display On
	0x20,	// Set display mode
	0x00,	// Horizontal addressing mode
};

//---------------------------------------------------------------------------
ST_OLED Oled;


//---------------------------------------------------------------------------
void OledInit(void)
{
	_Memset(&Oled, 0x00, sizeof(ST_OLED));


	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);

	pinMode(OLED_PIN_DC,  OUTPUT);
	pinMode(OLED_PIN_CS,  OUTPUT);
	pinMode(OLED_PIN_RST, OUTPUT);

	// VDD (3.3V) goes high at start, lets just chill for a ms
	digitalWrite(OLED_PIN_RST, HIGH);
	delay(1);

	// bring reset low. wait 10ms
	digitalWrite(OLED_PIN_RST, LOW);
	delay(10);

	// bring out of reset
	digitalWrite(OLED_PIN_RST, HIGH);

	// setup the ports we need to talk to the OLED
	vu8* pCs    = portOutputRegister(digitalPinToPort(OLED_PIN_CS));
	vu8* pDc    = portOutputRegister(digitalPinToPort(OLED_PIN_DC));
	u8   csMask = digitalPinToBitMask(OLED_PIN_CS);
	u8   dcMask = digitalPinToBitMask(OLED_PIN_DC);

	// Command Mode
	*pCs |=  csMask;
	*pDc &= ~dcMask;
	*pCs &= ~csMask;

	// Send boot Program
	u8 i;

	for(i=0; i<sizeof(OledBootProgram); i++)
	{
		SPI.transfer(__LPM(OledBootProgram + i));
	}

	// Data Mode
	*pDc |=  dcMask;
	*pCs &= ~csMask;
}
//---------------------------------------------------------------------------
void OledDisplay(void)
{
	u16 i;

	for(i=0; i<sizeof(Oled.buf); i++)
	{
		SPI.transfer(Oled.buf[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawStr(u8 fx, u8 fy, const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);


	u8 i;

	for(i=0; s[i] != '\0'; i++)
	{
		OledDrawChr(fx++, fy, s[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawStrP(u8 fx, u8 fy, const char* s)
{
	u8 c;

	while((c = __LPM(s++)) != '\0')
	{
		OledDrawChr(fx++, fy, c);
	}
}
//---------------------------------------------------------------------------
void OledDrawChr(u8 fx, u8 fy, char chr)
{
	if(fx >= OLED_SCREEN_FONT_CX || fy >= OLED_SCREEN_FONT_CY)
	{
		return;
	}

	u8 x;

	for(x=0; x<OLED_FONT_SIZE; x++)
	{
		Oled.buf[(fx * OLED_FONT_CX + x) + (fy * OLED_SCREEN_CX)] = __LPM(font + (chr * OLED_FONT_SIZE) + x);
	}
}
//---------------------------------------------------------------------------
void OledDrawDot(u8 x, u8 y, u8 col)
{
	if(x >= OLED_SCREEN_CX || y >= OLED_SCREEN_CY)
	{
		return;
	}

	if(col)
	{
		Oled.buf[x + (y / 8) * OLED_SCREEN_CX] |=  _BV(y % 8);
	}
	else
	{
		Oled.buf[x + (y / 8) * OLED_SCREEN_CX] &= ~_BV(y % 8);
	}
}
//---------------------------------------------------------------------------
void OledDrawCls(void)
{
//	u16 i;
//	for(i=0; i<sizeof(Oled.buf); i++) Oled.buf[i] = 0x00;

	asm volatile(
		"movw  r30, %0                \n\t"
		"eor __tmp_reg__, __tmp_reg__ \n\t"
		"loop:                        \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"inc __tmp_reg__              \n\t"
		"brne loop                    \n\t"

		: : "r" (Oled.buf) : "r30","r31"
	);
}
//---------------------------------------------------------------------------
void OledDrawBmp(s8 sx, s8 sy, u8* p)
{
	u8* d  = (u8*)p;
	u8  cx = __LPM(d++);
	u8  cy = __LPM(d++);

	u8 chr, mask;
	u8 x, y, b;

	for(y=0; y<cy; y++)
	{
		if(sy + y < 0 || sy + y >= OLED_SCREEN_CY)
		{
			d += cx / 8;
			continue;
		}

		for(x=0; x<cx; x+=8)
		{
			chr  = __LPM(d++);
			mask = 0x80;

			for(b=0; b<8; b++)
			{
				if(sx + x + b >= 0 && sx + x + b < OLED_SCREEN_CX)
				{
					if(chr & mask)
					{
						Oled.buf[(sx + x + b) + ((sy + y) / 8) * OLED_SCREEN_CX] |= _BV((sy + y) & 0x7);
					}
				}

			 	mask >>=1;
			}
		}
	}
}
//---------------------------------------------------------------------------
void OledDrawPng8(s8 bx, s8 by, u8* p, u8 num)
{
	u8* d = (u8*)p + num * 8;
	u8  i;

	for(i=0; i<8; i++)
	{
		Oled.buf[(bx * 8 + by * OLED_SCREEN_CX) + i] |= __LPM(d + i);
	}
}
