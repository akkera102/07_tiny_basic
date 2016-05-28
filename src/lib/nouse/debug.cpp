#include "debug.h"
#include <arduino.h>

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void DebugInit(void)
{
//	Serial.begin(9600);
}
//---------------------------------------------------------------------------
void DebugPrint(const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);

//	Serial.print(s);
}
