#include "ino.h"
#include "lib/sys.h"
#include "bas.h"

//---------------------------------------------------------------------------
void setup(void)
{
	SysInit();

	BasInit();
}
//---------------------------------------------------------------------------
void loop(void)
{
	if(SysLoop() == FALSE)
	{
		return;
	}

	BasLoop();

	SysLoopEnd();
}
