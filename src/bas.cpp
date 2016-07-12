#include "bas.h"
#include <arduino.h>
#include "lib/oled.h"
#include "lib/rnd.h"

//---------------------------------------------------------------------------
PROGMEM const char BasCmd00[] = "GOTO";
PROGMEM const char BasCmd01[] = "GOSUB";
PROGMEM const char BasCmd02[] = "RETURN";
PROGMEM const char BasCmd03[] = "FOR";
PROGMEM const char BasCmd04[] = "TO";
PROGMEM const char BasCmd05[] = "STEP";
PROGMEM const char BasCmd06[] = "NEXT";
PROGMEM const char BasCmd07[] = "IF";
PROGMEM const char BasCmd08[] = "REM";
PROGMEM const char BasCmd09[] = "STOP";
PROGMEM const char BasCmd10[] = "INPUT";
PROGMEM const char BasCmd11[] = "PRINT";
PROGMEM const char BasCmd12[] = "LET";
PROGMEM const char BasCmd13[] = ",";
PROGMEM const char BasCmd14[] = ";";
PROGMEM const char BasCmd15[] = "-";
PROGMEM const char BasCmd16[] = "+";
PROGMEM const char BasCmd17[] = "*";
PROGMEM const char BasCmd18[] = "/";
PROGMEM const char BasCmd19[] = "%";
PROGMEM const char BasCmd20[] = "(";
PROGMEM const char BasCmd21[] = ")";
PROGMEM const char BasCmd22[] = ">=";
PROGMEM const char BasCmd23[] = "#";
PROGMEM const char BasCmd24[] = ">";
PROGMEM const char BasCmd25[] = "=";
PROGMEM const char BasCmd26[] = "<=";
PROGMEM const char BasCmd27[] = "<";
PROGMEM const char BasCmd28[] = "@";
PROGMEM const char BasCmd29[] = "RND";
PROGMEM const char BasCmd30[] = "ABS";
PROGMEM const char BasCmd31[] = "SIZE";
PROGMEM const char BasCmd32[] = "LIST";
PROGMEM const char BasCmd33[] = "RUN";
PROGMEM const char BasCmd34[] = "NEW";
PROGMEM const char BasCmd35[] = "CLS";
PROGMEM const char BasCmd36[] = "PSET";

PROGMEM const char* const BasCmdTable[] = {
	BasCmd00,
	BasCmd01,
	BasCmd02,
	BasCmd03,
	BasCmd04,
	BasCmd05,
	BasCmd06,
	BasCmd07,
	BasCmd08,
	BasCmd09,
	BasCmd10,
	BasCmd11,
	BasCmd12,
	BasCmd13,
	BasCmd14,
	BasCmd15,
	BasCmd16,
	BasCmd17,
	BasCmd18,
	BasCmd19,
	BasCmd20,
	BasCmd21,
	BasCmd22,
	BasCmd23,
	BasCmd24,
	BasCmd25,
	BasCmd26,
	BasCmd27,
	BasCmd28,
	BasCmd29,
	BasCmd30,
	BasCmd31,
	BasCmd32,
	BasCmd33,
	BasCmd34,
	BasCmd35,
	BasCmd36,
};

PROGMEM const char BasErr00[] = "OK";
PROGMEM const char BasErr01[] = "Devision by zero";
PROGMEM const char BasErr02[] = "Overflow";
PROGMEM const char BasErr03[] = "Subscript out of range";
PROGMEM const char BasErr04[] = "Icode buffer full";
PROGMEM const char BasErr05[] = "List full";
PROGMEM const char BasErr06[] = "GOSUB too many nested";
PROGMEM const char BasErr07[] = "RETURN stack underflow";
PROGMEM const char BasErr08[] = "FOR too many nested";
PROGMEM const char BasErr09[] = "NEXT without FOR";
PROGMEM const char BasErr10[] = "NEXT without counter";
PROGMEM const char BasErr11[] = "NEXT mismatch FOR";
PROGMEM const char BasErr12[] = "FOR without variable";
PROGMEM const char BasErr13[] = "FOR without TO";
PROGMEM const char BasErr14[] = "LET without variable";
PROGMEM const char BasErr15[] = "IF without condition";
PROGMEM const char BasErr16[] = "Undefined line number";
PROGMEM const char BasErr17[] = "\'(\' or \')\' expected";
PROGMEM const char BasErr18[] = "\'=\' expected";
PROGMEM const char BasErr19[] = "Illegal command";
PROGMEM const char BasErr20[] = "Syntax error";
PROGMEM const char BasErr21[] = "Internal error";
PROGMEM const char BasErr22[] = "Abort by [ESC]";

PROGMEM const char* const BasErrTable[] = {
	BasErr00,
	BasErr01,
	BasErr02,
	BasErr03,
	BasErr04,
	BasErr05,
	BasErr06,
	BasErr07,
	BasErr08,
	BasErr09,
	BasErr10,
	BasErr11,
	BasErr12,
	BasErr13,
	BasErr14,
	BasErr15,
	BasErr16,
	BasErr17,
	BasErr18,
	BasErr19,
	BasErr20,
	BasErr21,
	BasErr22,
};

PROGMEM const u8 BasNoSpaceAfterTable[] = {
	BAS_CODE_RETURN,
	BAS_CODE_STOP,
	BAS_CODE_COMMA,
	BAS_CODE_MINUS,
	BAS_CODE_PLUS,
	BAS_CODE_MUL,
	BAS_CODE_DIV,
	BAS_CODE_OPEN,
	BAS_CODE_CLOSE,
	BAS_CODE_GTE,
	BAS_CODE_NEQ,
	BAS_CODE_GT,
	BAS_CODE_EQ,
	BAS_CODE_LTE,
	BAS_CODE_LT,
	BAS_CODE_ARRAY,
	BAS_CODE_RND,
	BAS_CODE_ABS,
	BAS_CODE_SIZE,
};

PROGMEM const u8 BasNoSpaceBeforeTable[] = {
	BAS_CODE_MINUS,
	BAS_CODE_PLUS,
	BAS_CODE_MUL,
	BAS_CODE_DIV,
	BAS_CODE_MOD,
	BAS_CODE_OPEN,
	BAS_CODE_CLOSE,
	BAS_CODE_GTE,
	BAS_CODE_NEQ,
	BAS_CODE_GT,
	BAS_CODE_EQ,
	BAS_CODE_LTE,
	BAS_CODE_LT,
	BAS_CODE_COMMA,
	BAS_CODE_SEMI,
	BAS_CODE_EOL,
};

PROGMEM const char BasMsg00[] = "TOYOSHIKI Tiny Basic";
PROGMEM const char BasMsg01[] = "Arduboy Edition";
PROGMEM const char BasMsg02[] = "1. Connect Serial Term.";
PROGMEM const char BasMsg03[] = "2. Press Any key.";
PROGMEM const char BasMsg04[] = "Coonect!";


//---------------------------------------------------------------------------
ST_BAS Bas;


//---------------------------------------------------------------------------
void BasInit(void)
{
	_Memset(&Bas, 0x00, sizeof(ST_BAS));

	Serial.begin(9600);
}
//---------------------------------------------------------------------------
void BasLoop(void)
{
	switch(Bas.act)
	{
	case BAS_EXEC_RESET:
		BasExecReset();
		break;

	case BAS_EXEC_RDY:
		BasExecRdy();
		break;

	case BAS_EXEC_KEY:
		BasExecKey();
		break;

	case BAS_EXEC_LEX:
		BasExecLex();
		break;

	case BAS_EXEC_ADR:
		BasExecAdr();
		break;

	case BAS_EXEC_CMD:
		BasExecCmd();
		break;

	case BAS_EXEC_RUN:
		BasExecRun();
		break;

	case BAS_EXEC_OK:
		BasExecOk();
		break;

	default:
		ERROR(Game.act);
		break;
	}

	if(BasIsError() == TRUE)
	{
		BasExecError();
	}
}
//---------------------------------------------------------------------------
void BasExecReset(void)
{
	OledDrawCls();

	OledDrawStrP(0, 0, BasMsg00);
	OledDrawStrP(0, 1, BasMsg01);
	OledDrawStrP(0, 3, BasMsg02);
	OledDrawStrP(0, 4, BasMsg03);

	if(BasSerialIsAvailable() == TRUE)
	{
		BasSerialRead();
		OledDrawStrP(0, 6, BasMsg04);

		Bas.act = BAS_EXEC_OK;
	}
}
//---------------------------------------------------------------------------
void BasExecRdy(void)
{
	BasSerialWriteChr('>');

	Bas.lineLen = 0;
	Bas.codeLen = 0;

	Bas.act = BAS_EXEC_KEY;
}
//---------------------------------------------------------------------------
void BasExecKey(void)
{
	if(BasSerialIsAvailable() == FALSE)
	{
		return;
	}

	u8 c = BasSerialRead();


	if(c == BAS_CODE_CR)
	{
		BasSerialWriteNewLine();
		Bas.lineBuf[Bas.lineLen++] = '\0';

		Bas.act = BAS_EXEC_LEX;
		return;
	}

	if(BasIsPrint(c) == TRUE && Bas.lineLen < BAS_SIZE_LINE - 2)
	{
		BasSerialWriteChr(c);
		Bas.lineBuf[Bas.lineLen++] = c;

		return;
	}

	if(c == BAS_CODE_BS && Bas.lineLen > 0)
	{
		BasSerialWriteChr(BAS_CODE_BS);
		BasSerialWriteChr(BAS_CODE_SP);
		BasSerialWriteChr(BAS_CODE_BS);
		Bas.lineLen--;

		return;
	}
}
//---------------------------------------------------------------------------
void BasExecLex(void)
{
	u8* p = Bas.lineBuf;

	for(;;)
	{
		if(BasIsError() == TRUE)
		{
			return;
		}

		p = BasSkipSpace(p);

		// EOL
		if(*p == '\0')
		{
			BasAddCode(BAS_CODE_EOL);

			if(Bas.codeBuf[0] == BAS_CODE_NUM)
			{
				Bas.act = BAS_EXEC_ADR;
			}
			else
			{
				Bas.act = BAS_EXEC_CMD;
			}

			return;
		}

		// CMD
		if(BasIsCmd(p) == TRUE)
		{
			p = BasAddCodeCmd(p);
			continue;
		}

		// NUM
		if(BasIsDigit(*p) == TRUE)
		{
			p = BasAddCodeNum(p);
			continue;
		}

		// STR
		if(BasIsString(*p) == TRUE)
		{
			p = BasAddCodeStr(p);
			continue;
		}

		// VAR
		if(BasIsAlpha(*p) == TRUE)
		{
			p = BasAddCodeVar(p);
			continue;
		}

		// ERR
		Bas.error = BAS_ERROR_SYNTAX;
		return;
	}
}
//---------------------------------------------------------------------------
void BasExecAdr(void)
{
	if(Bas.codeLen + Bas.listLen >= BAS_SIZE_LIST)
	{
		Bas.error = BAS_ERROR_LBUFOF;
		return;
	}


	s16 no = BasGetLineNo(Bas.codeBuf);
	u8* p  = BasGetListPointer(no);

	u8* p1;
	u8* p2;
	s16 len;

	if(BasGetLineNo(p) == no)
	{
		Bas.listLen -= *p;

		p1 = p;
		p2 = p + *p;

		// sort and delete line
		while((len = *p2) != 0)
		{
			while(len--)
			{
				*p1++ = *p2++;
			}
		}

		*p1 = 0;
	}

	// case line number
	if(Bas.codeLen == 4)
	{
		Bas.act = BAS_EXEC_RDY;
		return;
	}

	// make space
	for(p1 = p; *p1; p1 += *p1)
	{
		// EMPTY
	}
	len = p1 - p + 1;
	p2  = p1 + Bas.codeLen;

	while(len--)
	{
		*p2-- = *p1--;
	}

	// insert
	p1  = p;
	p2  = Bas.codeBuf;
	len = Bas.codeLen;

	Bas.codeBuf[0] = Bas.codeLen;

	while(len--)
	{
		*p1++ = *p2++;
	}

	Bas.listLen += Bas.codeLen;
	Bas.act = BAS_EXEC_RDY;
}
//---------------------------------------------------------------------------
void BasExecCmd(void)
{
	s16 no = 0;
	u8* p;

	switch(Bas.codeBuf[0])
	{
	case BAS_CODE_NEW:
		if(Bas.codeBuf[1] != BAS_CODE_EOL)
		{
			Bas.error = BAS_ERROR_SYNTAX;

			return;
		}

		BasInit();
		Bas.act = BAS_EXEC_OK;
		break;

	case BAS_CODE_LIST:
		if(Bas.codeBuf[1] != BAS_CODE_EOL && Bas.codeBuf[4] != BAS_CODE_EOL)
		{
			Bas.error = BAS_ERROR_SYNTAX;

			return;
		}

		if(Bas.codeBuf[1] == BAS_CODE_NUM)
		{
			no = BasGetLineNo(&Bas.codeBuf[1]);
		}

		p = BasGetListPointer(no);

		while(*p)
		{
			BasSerialWriteNum(BasGetLineNo(p), 0);
			BasSerialWriteChr(' ');
			BasSerialWriteList(p + 3);
			BasSerialWriteNewLine();

			p += *p;
		}

		Bas.act = BAS_EXEC_OK;
		break;

	case BAS_CODE_EOL:
		Bas.act = BAS_EXEC_OK;
		break;

	case BAS_CODE_RUN:
		if(Bas.codeBuf[1] != BAS_CODE_EOL)
		{
			Bas.error = BAS_ERROR_SYNTAX;

			return;
		}

		if(Bas.listLen == 0)
		{
			Bas.act = BAS_EXEC_OK;

			return;
		}

		Bas.forIdx   = 0;
		Bas.gosubIdx = 0;

		Bas.pLine = Bas.listBuf;
		Bas.pCur  = Bas.listBuf + 3;
		Bas.act   = BAS_EXEC_RUN;
		break;

	default:
		Bas.pLine = Bas.codeBuf;
		Bas.pCur  = Bas.codeBuf;
		Bas.act   = BAS_EXEC_RUN;
		break;
	}
}
//---------------------------------------------------------------------------
void BasExecRun(void)
{
	if(BasSerialIsAvailable() == TRUE)
	{
		if(BasSerialRead() == BAS_CODE_ESC)
		{ 
			Bas.error = BAS_ERROR_ESC;
			return;
		}
	}

	if(*Bas.pCur == BAS_CODE_EOL)
	{
		if(BasIsRunList() == FALSE)
		{
			Bas.act = BAS_EXEC_OK;
			return;
		}

		Bas.pLine += *Bas.pLine;
		Bas.pCur   =  Bas.pLine + 3;

		if(*Bas.pLine == 0)
		{
			Bas.act = BAS_EXEC_OK;
			return;
		}
	}

	switch(*Bas.pCur++)
	{
	case BAS_CODE_GOTO:
		BasCmdGoto();
		break;

	case BAS_CODE_GOSUB:
		BasCmdGosub();
		break;

	case BAS_CODE_RETURN:
		BasCmdReturn();
		break;

	case BAS_CODE_FOR:
		BasCmdFor();
		break;

	case BAS_CODE_NEXT:
		BasCmdNext();
		break;

	case BAS_CODE_IF:
		BasCmdIf();
		break;

	case BAS_CODE_REM:
		BasCmdRem();
		break;

	case BAS_CODE_STOP:
		BasCmdStop();
		break;

	case BAS_CODE_VAR:
		BasCmdVar();
		break;

	case BAS_CODE_ARRAY:
		BasCmdArray();
		break;

	case BAS_CODE_LET:
		BasCmdLet();
		break;

	case BAS_CODE_PRINT:
		BasCmdPrint();
		break;

	case BAS_CODE_INPUT:
		BasCmdInput();
		break;

	case BAS_CODE_SEMI:
		// EMPTY
		break;

	case BAS_CODE_CLS:
		BasCmdCls();
		break;

	case BAS_CODE_PSET:
		BasCmdPset();
		break;


	case BAS_CODE_LIST:
	case BAS_CODE_NEW:
	case BAS_CODE_RUN:
		Bas.error = BAS_ERROR_COM;
		break;

	default:
		Bas.error = BAS_ERROR_SYNTAX;
		break;
	}
}
//---------------------------------------------------------------------------
void BasExecOk(void)
{
	BasSerialWriteNewLine();
	BasSerialWriteStrP(__LPM_word(BasErrTable + 0));
	BasSerialWriteNewLine();

	Bas.act = BAS_EXEC_RDY;
}
//---------------------------------------------------------------------------
void BasExecError(void)
{
	BasSerialWriteNewLine();

	if(Bas.act == BAS_EXEC_RUN && BasIsRunList() == TRUE)
	{
		BasSerialWriteStr((char*)"LINE:");
		BasSerialWriteNum(BasGetLineNo(Bas.pLine), 0);
		BasSerialWriteChr(' ');

		BasSerialWriteList(Bas.pLine + 3);
	}
	else
	{
		BasSerialWriteStr((char*)"YOU TYPE:");
		BasSerialWriteStr((char*)Bas.lineBuf);
	}

	BasSerialWriteNewLine();
	BasSerialWriteStrP(__LPM_word(BasErrTable + Bas.error));
	BasSerialWriteNewLine();

	Bas.error = BAS_ERROR_OK;
	Bas.act   = BAS_EXEC_RDY;
}
//---------------------------------------------------------------------------
void BasCmdGoto(void)
{
	s16 num = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	u8* p = BasGetListPointer(num);

	if(num != BasGetLineNo(p))
	{
		Bas.error = BAS_ERROR_ULN;
		return;
	}

	Bas.pLine = p;
	Bas.pCur  = p + 3;
}
//---------------------------------------------------------------------------
void BasCmdGosub(void)
{
	s16 num = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	u8* p = BasGetListPointer(num);

	if(num != BasGetLineNo(p))
	{
		Bas.error = BAS_ERROR_ULN;
		return;
	}

	if(Bas.gosubIdx > BAS_SIZE_GOSUB - 2)
	{
		Bas.error = BAS_ERROR_GSTKOF;
		return;
	}

	Bas.gosubStack[Bas.gosubIdx++] = Bas.pCur;
	Bas.gosubStack[Bas.gosubIdx++] = Bas.pLine;

	Bas.pLine = p;
	Bas.pCur  = p + 3;
}
//---------------------------------------------------------------------------
void BasCmdReturn(void)
{
	if(Bas.gosubIdx < 2)
	{
		Bas.error = BAS_ERROR_GSTKUF;
		return;
	}

	Bas.pLine = Bas.gosubStack[--Bas.gosubIdx];
	Bas.pCur  = Bas.gosubStack[--Bas.gosubIdx];
}
//---------------------------------------------------------------------------
void BasCmdFor(void)
{
	s16 idx, to, step;

	// VAR
	if(*Bas.pCur++ != BAS_CODE_VAR)
	{
		Bas.error = BAS_ERROR_FORWOV;
		return;
	}
	idx = *Bas.pCur;

	BasCmdVar();

	if(BasIsError() == TRUE)
	{
		return;
	}

	// TO
	if(*Bas.pCur++ != BAS_CODE_TO)
	{
		Bas.error = BAS_ERROR_FORWOTO;
		return;
	}
	to = BasCalcCond();

	// STEP
	if(*Bas.pCur == BAS_CODE_STEP)
	{
		Bas.pCur++;
		step = BasCalcCond();
	}
	else
	{
		step = 1;
	}

	if(((step < 0) && (-32767 - step > to)) || ((step > 0) && (32767 - step < to)))
	{
		Bas.error = BAS_ERROR_VOF;
		return;
	}

	if(Bas.forIdx > BAS_SIZE_FOR - 5)
	{
		Bas.error = BAS_ERROR_LSTKOF;
		return;
	}

	Bas.forStack[Bas.forIdx++] = Bas.pLine;
	Bas.forStack[Bas.forIdx++] = Bas.pCur;
	Bas.forStack[Bas.forIdx++] = (u8*)(s16)to;
	Bas.forStack[Bas.forIdx++] = (u8*)(s16)step;
	Bas.forStack[Bas.forIdx++] = (u8*)(s16)idx;
}
//---------------------------------------------------------------------------
void BasCmdNext(void)
{
	if(Bas.forIdx < 5)
	{
		Bas.error = BAS_ERROR_LSTKUF;
		return;
	}

	s16 idx  = (s16)Bas.forStack[Bas.forIdx - 1];
	s16 step = (s16)Bas.forStack[Bas.forIdx - 2];
	s16 to   = (s16)Bas.forStack[Bas.forIdx - 3];

	if(*Bas.pCur++ != BAS_CODE_VAR)
	{
		Bas.error = BAS_ERROR_NEXTWOV;
		return;
	}

	if(*Bas.pCur++ != idx)
	{
		Bas.error = BAS_ERROR_NEXTUM;
		return;
	}

	Bas.var[idx] += step;

	if(((step < 0) && (Bas.var[idx] < to)) || ((step > 0) && (Bas.var[idx] > to)))
	{
		Bas.forIdx -= 5;
	}
	else
	{
		Bas.pCur  = Bas.forStack[Bas.forIdx - 4];
		Bas.pLine = Bas.forStack[Bas.forIdx - 5];
	}
}
//---------------------------------------------------------------------------
void BasCmdIf(void)
{
	s16 cond = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		Bas.error = BAS_ERROR_IFWOC;
		return;
	}

	if(cond == 0)
	{
		BasCmdRem();
	}
}
//---------------------------------------------------------------------------
void BasCmdRem(void)
{
	while(*Bas.pCur != BAS_CODE_EOL)
	{
		Bas.pCur++;
	}
}
//---------------------------------------------------------------------------
void BasCmdStop(void)
{
	while(*Bas.pLine)
	{
		Bas.pLine += *Bas.pLine;
	}
}
//---------------------------------------------------------------------------
void BasCmdVar(void)
{
	u8 idx = *Bas.pCur++;

	if(*Bas.pCur++ != BAS_CODE_EQ)
	{
		Bas.error = BAS_ERROR_VWOEQ;
		return;
	}

	s16 val = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	Bas.var[idx] = val;
}
//---------------------------------------------------------------------------
void BasCmdArray(void)
{
	s16 idx = BasCalcParam();

	if(BasIsError() == TRUE)
	{
		return;
	}

	if(idx >= BAS_SIZE_ARRAY)
	{
		Bas.error = BAS_ERROR_SOR;
		return;
	}

	if(*Bas.pCur++ != BAS_CODE_EQ)
	{
		Bas.error = BAS_ERROR_VWOEQ;
		return;
	}

	s16 val = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	Bas.array[idx] = val;
}
//---------------------------------------------------------------------------
void BasCmdLet(void)
{
	u8 code = *Bas.pCur++;

	if(code == BAS_CODE_VAR)
	{
		BasCmdVar();
		return;
	}

	if(code == BAS_CODE_ARRAY)
	{
		BasCmdArray();
		return;
	}

	Bas.error = BAS_ERROR_LETWOV;
}
//---------------------------------------------------------------------------
void BasCmdPrint(void)
{
	s16 len = 0;
	s16 i;

	while(*Bas.pCur != BAS_CODE_SEMI && *Bas.pCur != BAS_CODE_EOL)
	{
		switch(*Bas.pCur)
		{
		case BAS_CODE_STR:
			Bas.pCur++;

			i = *Bas.pCur++;

			while(i--)
			{
				BasSerialWriteChr(*Bas.pCur++);
			}
			break;

		case BAS_CODE_NEQ:
			Bas.pCur++;

			len = BasCalcCond();

			if(BasIsError() == TRUE)
			{
				return;
			}
			break;

		default: ;
			s16 val = BasCalcCond();

			if(BasIsError() == TRUE)
			{
				return;
			}

			BasSerialWriteNum(val, len);
			break;
		}

		if(*Bas.pCur == BAS_CODE_COMMA)
		{
			Bas.pCur++;

			if(*Bas.pCur == BAS_CODE_SEMI || *Bas.pCur == BAS_CODE_EOL)
			{
				return;
			}
		}
		else if(*Bas.pCur != BAS_CODE_SEMI && *Bas.pCur != BAS_CODE_EOL)
		{
			Bas.error = BAS_ERROR_SYNTAX;
			return;
		}
	}

	BasSerialWriteNewLine();
}
//---------------------------------------------------------------------------
void BasCmdInput(void)
{
	// TODO
	Bas.error = BAS_ERROR_SYNTAX;
}
//---------------------------------------------------------------------------
void BasCmdCls(void)
{
	OledDrawCls();
}
//---------------------------------------------------------------------------
void BasCmdPset(void)
{
	// x
	s16 x = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	// ,
	if(*Bas.pCur++ != BAS_CODE_COMMA)
	{
		Bas.error = BAS_ERROR_SYNTAX;
		return;
	}

	// y
	s16 y = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	// ,
	if(*Bas.pCur++ != BAS_CODE_COMMA)
	{
		Bas.error = BAS_ERROR_SYNTAX;
		return;
	}

	// col
	s16 col = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return;
	}

	OledDrawDot(x, y, col);
}
//---------------------------------------------------------------------------
s16 BasCalcCond(void)
{
	s16 val = BasCalcExpr();

	if(BasIsError() == TRUE)
	{
		return -1;
	}

	for(;;)
	{
		switch(*Bas.pCur)
		{
		case BAS_CODE_EQ:
			Bas.pCur++;
			val = (val == BasCalcExpr());
			break;

		case BAS_CODE_NEQ:
			Bas.pCur++;
			val = (val != BasCalcExpr());
			break;

		case BAS_CODE_LT:
			Bas.pCur++;
			val = (val <  BasCalcExpr());
			break;

		case BAS_CODE_LTE:
			Bas.pCur++;
			val = (val <= BasCalcExpr());
			break;

		case BAS_CODE_GT:
			Bas.pCur++;
			val = (val >  BasCalcExpr());
			break;

		case BAS_CODE_GTE:
			Bas.pCur++;
			val = (val >= BasCalcExpr());
			break;

		default:
			return val;
		}
	}
}
//---------------------------------------------------------------------------
s16 BasCalcExpr(void)
{
	s16 val = BasCalcTerm();

	if(BasIsError() == TRUE)
	{
		return -1;
	}

	for(;;)
	{
		switch(*Bas.pCur)
		{
		case BAS_CODE_PLUS:
			Bas.pCur++;
			val += BasCalcTerm();
			break;

		case BAS_CODE_MINUS:
			Bas.pCur++;
			val -= BasCalcTerm();
			break;

		default:
			return val;
		}
	}
}
//---------------------------------------------------------------------------
s16 BasCalcTerm(void)
{
	s16 val = BasCalcFactor();
	s16 tmp;

	if(BasIsError() == TRUE)
	{
		return -1;
	}

	for(;;)
	{
		switch(*Bas.pCur)
		{
		case BAS_CODE_MUL:
			Bas.pCur++;
			val *= BasCalcFactor();
			break;

		case BAS_CODE_MOD:
			Bas.pCur++;
			val %= BasCalcFactor();
			break;

		case BAS_CODE_DIV:
			Bas.pCur++;
			tmp = BasCalcFactor();

			if(tmp == 0)
			{
				Bas.error = BAS_ERROR_DIVBY0;
				return -1;
			}

			val /= tmp;
			break;

		default:
			return val;
		}
	}
}
//---------------------------------------------------------------------------
s16 BasCalcFactor(void)
{
	s16 val = 0;

	switch(*Bas.pCur)
	{
	case BAS_CODE_NUM:
		Bas.pCur++;
		val = *Bas.pCur | *(Bas.pCur + 1) << 8;
		Bas.pCur += 2;
		break;

	case BAS_CODE_PLUS:
		Bas.pCur++;
		val = BasCalcFactor();
		break;

	case BAS_CODE_MINUS:
		Bas.pCur++;
		val = 0 - BasCalcFactor();
		break;

	case BAS_CODE_VAR:
		Bas.pCur++;
		val = Bas.var[*Bas.pCur++];
		break;

	case BAS_CODE_OPEN:
		val = BasCalcParam();
		break;

	case BAS_CODE_ARRAY:
		Bas.pCur++;
		val = BasCalcParam();

		if(BasIsError() == TRUE)
		{
			break;
		}

		if(val >= BAS_SIZE_ARRAY)
		{
			Bas.error = BAS_ERROR_SOR;
			break;
		}

		val = Bas.array[val];
		break;

	case BAS_CODE_RND:
		Bas.pCur++;
		val = BasCalcParam();

		if(BasIsError() == TRUE)
		{
			break;
		}

		val = BasCalcRnd(val);
		break;

	case BAS_CODE_ABS:
		Bas.pCur++;
		val = BasCalcParam();

		if(BasIsError() == TRUE)
		{
			break;
		}

		if(val < 0)
		{
			val *= -1;
		}
		break;

	case BAS_CODE_SIZE:
		Bas.pCur++;

		if((*Bas.pCur != BAS_CODE_OPEN) || (*(Bas.pCur + 1) != BAS_CODE_CLOSE))
		{
			Bas.error = BAS_ERROR_PAREN;
			break;
		}
		Bas.pCur += 2;

		val = Bas.listLen;
		break;

	default:
		Bas.error = BAS_ERROR_SYNTAX;
		break;
	}

	return val;
}
//---------------------------------------------------------------------------
s16 BasCalcParam(void)
{
	// (
	if(*Bas.pCur++ != BAS_CODE_OPEN)
	{
		Bas.error = BAS_ERROR_PAREN;
		return 0;
	}

	s16 val = BasCalcCond();

	if(BasIsError() == TRUE)
	{
		return 0;
	}

	// )
	if(*Bas.pCur++ != BAS_CODE_CLOSE)
	{
		Bas.error = BAS_ERROR_PAREN;
		return 0;
	}

	return val;
}
//---------------------------------------------------------------------------
s16 BasCalcRnd(s16 val)
{
	if(val == 0)
	{
		return 0;
	}

	if(val < 0)
	{
		val *= -1;
	}

	return Rnd(val) + 1;
}
//---------------------------------------------------------------------------
void BasSerialWriteChr(u8 c)
{
	Serial.write((char)c);
}
//---------------------------------------------------------------------------
void BasSerialWriteStr(char* s)
{
	while(*s != '\0')
	{
		BasSerialWriteChr((u8)*s++);
	}
}
//---------------------------------------------------------------------------
void BasSerialWriteStrP(u16 s)
{
	u8 c;

	while((c = __LPM(s)) != '\0')
	{
		BasSerialWriteChr(c);
		s++;
	}
}
//---------------------------------------------------------------------------
void BasSerialWriteNum(s16 val, s16 d)
{
	bool is;

	if(val < 0)
	{
		is  = TRUE;
		val = -val;
	}
	else
	{
		is  = FALSE;
	}

	char buf[7];
	buf[6] = 0;

	u8 dig;
	dig = 6;

	do {
		buf[--dig] = (val % 10) + '0';
		val /= 10;

	} while(val > 0);

	if(is == TRUE)
	{
		buf[--dig] = '-';
	}

	while(6 - dig < d)
	{
		BasSerialWriteChr(' ');
		d--;
	}

	BasSerialWriteStr(&buf[dig]);
}
//---------------------------------------------------------------------------
void BasSerialWriteList(u8* p)
{
	s16 i;

	while(*p != BAS_CODE_EOL)
	{
		// CMD
		if(*p < BAS_SIZE_CMD)
		{
			BasSerialWriteStrP(__LPM_word(BasCmdTable + *p));

			if(BasIsNoSpaceAfter(*p) == FALSE)
			{
				BasSerialWriteChr(' ');
			}

			if(*p == BAS_CODE_REM)
			{
				p++;
				i = *p++;

				while(i--)
				{
					BasSerialWriteChr(*p++);
				}

				return;
			}

			p++;
			continue;
		}

		// NUM
		if(*p == BAS_CODE_NUM)
		{
			p++;
			BasSerialWriteNum(*p | *(p + 1) << 8, 0);
			p += 2;

			if(BasIsNoSpaceBefore(*p) == FALSE)
			{
				BasSerialWriteChr(' ');
			}

			continue;
		}

		// VAR
		if(*p == BAS_CODE_VAR)
		{
			p++;
			BasSerialWriteChr(*p++ + 'A');

			if(BasIsNoSpaceBefore(*p) == FALSE)
			{
				BasSerialWriteChr(' ');
			}

			continue;
		}

		// STR
		if(*p == BAS_CODE_STR)
		{
			p++;

			BasSerialWriteChr('\"');

			i = *p++;
			while(i--)
			{
				BasSerialWriteChr(*p++);
			}

			BasSerialWriteChr('\"');

			if(*p == BAS_CODE_VAR)
			{
				BasSerialWriteChr(' ');
			}

			continue;
		}
	}
}
//---------------------------------------------------------------------------
void BasSerialWriteNewLine(void)
{
	BasSerialWriteChr(BAS_CODE_CR);
	BasSerialWriteChr(BAS_CODE_LF);
}
//---------------------------------------------------------------------------
u8 BasSerialRead(void)
{
	u8 c = (u8)Serial.read();

	if(c == BAS_CODE_TAB)
	{
		c = ' ';
	}

	if(c == BAS_CODE_DEL)
	{
		c = BAS_CODE_BS;
	}

	if(BasIsToupper(c) == TRUE)
	{
		c -= 0x20;
	}

	return c;
}
//---------------------------------------------------------------------------
bool BasSerialIsAvailable(void)
{
	return (Serial.available() > 0) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsNoSpaceAfter(u8 code)
{
	u8 i;

	for(i=0; i<sizeof(BasNoSpaceAfterTable); i++)
	{
		if(code == __LPM(BasNoSpaceAfterTable + i))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------
bool BasIsNoSpaceBefore(u8 code)
{
	u8 i;

	for(i=0; i<sizeof(BasNoSpaceBeforeTable); i++)
	{
		if(code == __LPM(BasNoSpaceBeforeTable + i))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------
bool BasIsPrint(u8 c)
{
	return (c >= 0x20 && c <= 0x7e) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsSpace(u8 c)
{
	return (c == ' ' || (c >= 0x09 && c <= 0x0d)) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsToupper(u8 c)
{
	return (c >= 'a' && c <= 'z') ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsAlpha(u8 c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsDigit(u8 c)
{
	return(c >= '0' && c <= '9') ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsString(u8 c)
{
	return(c == '\"' || c == '\'') ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsCmd(u8* p)
{
	return (BasGetCmdIdx(p) < BAS_SIZE_CMD) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsRunList(void)
{
	return (Bas.pCur >= Bas.listBuf && Bas.pCur < Bas.listBuf + BAS_SIZE_LIST) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BasIsError(void)
{
	return (Bas.error != BAS_ERROR_OK) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
void BasAddCode(u8 code)
{
	if(Bas.codeLen + 1 >= BAS_SIZE_CODE)
	{
		Bas.error = BAS_ERROR_IBUFOF;
		return;
	}

	Bas.codeBuf[Bas.codeLen++] = code;
}
//---------------------------------------------------------------------------
void BasAddCode2(u8* p, s16 len)
{
	s16 i;

	for(i=0; i<len; i++)
	{
		BasAddCode(*p++);
	}
}
//---------------------------------------------------------------------------
u8* BasAddCodeCmd(u8* p)
{
	u8 cmd = BasGetCmdIdx(p);
	BasAddCode(cmd);

	p += BasGetCmdLen(cmd);

	if(cmd == BAS_CODE_REM)
	{
		u8 i;

		p = BasSkipSpace(p);

		for(i=0; p[i] != '\0'; i++)
		{
			// EMPTY
		}

		BasAddCode(i);
		BasAddCode2(p, i);

		p += i;
	}

	return p;
}
//---------------------------------------------------------------------------
u8* BasAddCodeNum(u8* p)
{
	s16 val = 0;
	s16 tmp = 0;

	do
	{
		tmp = 10 * val + *p++ - '0';

		if(val > tmp)
		{
			Bas.error = BAS_ERROR_VOF;
			goto End;
		}
		val = tmp;

	} while(BasIsDigit(*p) == TRUE);

	BasAddCode(BAS_CODE_NUM);
	BasAddCode(val & 0xff);
	BasAddCode(val >> 8);

End:
	return p;
}
//---------------------------------------------------------------------------
u8* BasAddCodeStr(u8* p)
{
	// Skip " or '
	p++;

	u8  i;
	u8* t = p;

	// Get length
	for(i=0; BasIsString(*t) == FALSE && BasIsPrint(*t) == TRUE; i++)
	{
		t++;
	}

	BasAddCode(BAS_CODE_STR);
	BasAddCode(i);
	BasAddCode2(p, i);
	p += i;

	// Skip " or '
	if(BasIsString(*p) == TRUE)
	{
		p++; 
	}

	return p;
}
//---------------------------------------------------------------------------
u8* BasAddCodeVar(u8* p)
{
	BasAddCode(BAS_CODE_VAR);
	BasAddCode(*p++ - 'A');

	return p;
}
//---------------------------------------------------------------------------
u8* BasGetListPointer(s16 lineno)
{
	u8* p;

	for(p=Bas.listBuf; *p; p+=*p)
	{
		if(BasGetLineNo(p) >= lineno)
		{
			break;
		}
	}

	return p;
}
//---------------------------------------------------------------------------
s16 BasGetLineNo(u8* p)
{
	if(*p == 0)
	{
		return 32767;
	}

	return *(p + 1) | *(p + 2) << 8;
}
//---------------------------------------------------------------------------
u8 BasGetCmdIdx(u8* p)
{
	u8* p1;
	u8* p2;
	u8  i;

	for(i=0; i<BAS_SIZE_CMD; i++)
	{
		p1 = (u8*)__LPM_word(BasCmdTable + i);;
		p2 = p;

		while(__LPM(p1) != '\0' && __LPM(p1) == *p2)
		{
			p1++;
			p2++;
		}

		if(__LPM(p1) == '\0')
		{
			goto End;
		}
	}

End:
	return i;
}
//---------------------------------------------------------------------------
u8 BasGetCmdLen(u8 idx)
{
	u8* p1 = (u8*)__LPM_word(BasCmdTable + idx);
	u8* p2 = (u8*)__LPM_word(BasCmdTable + idx);

	while(__LPM(p1++) != '\0')
	{
		// EMPTY
	}

	return p1 - p2 - 1;
}
//---------------------------------------------------------------------------
u8* BasSkipSpace(u8* p)
{
	while(BasIsSpace(*p) == TRUE)
	{
		p++;
	}

	return p;
}
