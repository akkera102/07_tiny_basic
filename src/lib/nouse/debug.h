#ifndef DEBUG_H
#define DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void DebugInit(void);
void DebugPrint(const char* fmt, ...);


#ifdef __cplusplus
}
#endif
#endif
