#ifndef EEP_H
#define EEP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
typedef struct {
	u8* adr;

} ST_EEP;


//---------------------------------------------------------------------------
void EepInit(void);
void EepSeek(u16 adr);

u8   EepRead8(void);
u16  EepRead16(void);
u32  EepRead32(void);
void EepWrite8(u8 dat);
void EepWrite16(u16 dat);
void EepWrite32(u32 dat);

void EepWriteAdr8(u8 adr, u8 dat);

#ifdef __cplusplus
}
#endif
#endif
