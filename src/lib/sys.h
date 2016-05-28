#ifndef SYS_H
#define SYS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void SysInit(void);
void SysIdle(void);
bool SysLoop(void);
void SysLoopEnd(void);


#ifdef __cplusplus
}
#endif
#endif
