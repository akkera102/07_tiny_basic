#include "eep.h"
#include <avr/eeprom.h>

//---------------------------------------------------------------------------
ST_EEP Eep;


//---------------------------------------------------------------------------
void EepInit(void)
{
	_Memset(&Eep, 0x00, sizeof(ST_EEP));
}
//---------------------------------------------------------------------------
void EepSeek(u16 adr)
{
	Eep.adr = (u8*)adr;
}
//---------------------------------------------------------------------------
u8 EepRead8(void)
{
	eeprom_busy_wait();
	return eeprom_read_byte(Eep.adr++);
}
//---------------------------------------------------------------------------
u16 EepRead16(void)
{
	u16 ret = 0;

	ret |= EepRead8();
	ret |= EepRead8() << 8;

	return ret;
}
//---------------------------------------------------------------------------
u32 EepRead32(void)
{
	u32 ret = 0;

	ret |= (u32)EepRead8();
	ret |= (u32)EepRead8() <<  8;
	ret |= (u32)EepRead8() << 16;
	ret |= (u32)EepRead8() << 24;

	return ret;
}
//---------------------------------------------------------------------------
void EepWrite8(u8 dat)
{
	eeprom_busy_wait();
	eeprom_write_byte(Eep.adr++, dat);
}
//---------------------------------------------------------------------------
void EepWrite16(u16 dat)
{
	EepWrite8(dat & 0xff);
	EepWrite8(dat >> 8);
}
//---------------------------------------------------------------------------
void EepWrite32(u32 dat)
{
	EepWrite8((dat      ) & 0xff);
	EepWrite8((dat >>  8) & 0xff);
	EepWrite8((dat >> 16) & 0xff);
	EepWrite8((dat >> 24) & 0xff);
}
//---------------------------------------------------------------------------
void EepWriteAdr8(u8 adr, u8 dat)
{
	EepSeek(adr);
	EepWrite8(dat);
}
