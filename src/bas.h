#ifndef BAS_H
#define BAS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------
#define BAS_CODE_CR			0x0d
#define BAS_CODE_LF			0x0a
#define BAS_CODE_TAB		0x09
#define BAS_CODE_BS			0x08
#define BAS_CODE_DEL		0x7f
#define BAS_CODE_SP			0x20
#define BAS_CODE_ESC		0x1b

#define BAS_SIZE_LINE		50
#define BAS_SIZE_CODE		50
#define BAS_SIZE_LIST		300
#define BAS_SIZE_ARRAY		20
#define BAS_SIZE_VAR		26
#define BAS_SIZE_GOSUB		6
#define BAS_SIZE_FOR		15
#define BAS_SIZE_CMD		(sizeof(BasCmdTable) / sizeof(const char*))

enum {
	BAS_EXEC_RESET,
	BAS_EXEC_RDY,
	BAS_EXEC_KEY,
	BAS_EXEC_LEX,
	BAS_EXEC_ADR,
	BAS_EXEC_CMD,
	BAS_EXEC_RUN,
	BAS_EXEC_OK,
};

enum {
	BAS_CODE_GOTO,
	BAS_CODE_GOSUB,
	BAS_CODE_RETURN,
	BAS_CODE_FOR,
	BAS_CODE_TO,
	BAS_CODE_STEP,
	BAS_CODE_NEXT,
	BAS_CODE_IF,
	BAS_CODE_REM,
	BAS_CODE_STOP,
	BAS_CODE_INPUT,			// 0x0a
	BAS_CODE_PRINT,
	BAS_CODE_LET,
	BAS_CODE_COMMA,
	BAS_CODE_SEMI,
	BAS_CODE_MINUS,
	BAS_CODE_PLUS,			// 0x10
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
	BAS_CODE_LT,			// 0x1b
	BAS_CODE_ARRAY,
	BAS_CODE_RND,
	BAS_CODE_ABS,
	BAS_CODE_SIZE,
	BAS_CODE_LIST,
	BAS_CODE_RUN,			// 0x21
	BAS_CODE_NEW,
	BAS_CODE_CLS,
	BAS_CODE_PSET,

	BAS_CODE_NUM,			// 0x25
	BAS_CODE_VAR,
	BAS_CODE_STR,
	BAS_CODE_EOL,			// 0x28
};

enum {
	BAS_ERROR_OK,
	BAS_ERROR_DIVBY0,
	BAS_ERROR_VOF,
	BAS_ERROR_SOR,
	BAS_ERROR_IBUFOF,
	BAS_ERROR_LBUFOF,
	BAS_ERROR_GSTKOF,
	BAS_ERROR_GSTKUF,
	BAS_ERROR_LSTKOF,
	BAS_ERROR_LSTKUF,
	BAS_ERROR_NEXTWOV,
	BAS_ERROR_NEXTUM,
	BAS_ERROR_FORWOV,
	BAS_ERROR_FORWOTO,
	BAS_ERROR_LETWOV,
	BAS_ERROR_IFWOC,
	BAS_ERROR_ULN,
	BAS_ERROR_PAREN,
	BAS_ERROR_VWOEQ,
	BAS_ERROR_COM,
	BAS_ERROR_SYNTAX,
	BAS_ERROR_SYS,
	BAS_ERROR_ESC
};
//---------------------------------------------------------------------------

typedef struct {
	u8   act;

	s16  lineLen;
	u8   lineBuf[BAS_SIZE_LINE];

	s16  codeLen;
	u8   codeBuf[BAS_SIZE_CODE];

	s16  listLen;
	u8   listBuf[BAS_SIZE_LIST];

	u8*  pCur;							// Pointer current
	u8*  pLine;							// Pointer current line

	s16  var[BAS_SIZE_VAR];
	s16  array[BAS_SIZE_ARRAY];

	s8   forIdx;
	u8*  forStack[BAS_SIZE_FOR];

	s8   gosubIdx;
	u8*  gosubStack[BAS_SIZE_GOSUB];

	u8   error;

} ST_BAS;


//---------------------------------------------------------------------------
void BasInit(void);
void BasLoop(void);

void BasExecReset(void);
void BasExecRdy(void);
void BasExecKey(void);
void BasExecLex(void);
void BasExecAdr(void);
void BasExecCmd(void);
void BasExecRun(void);
void BasExecOk(void);
void BasExecError(void);

void BasCmdGoto(void);
void BasCmdGosub(void);
void BasCmdReturn(void);
void BasCmdFor(void);
void BasCmdNext(void);
void BasCmdIf(void);
void BasCmdRem(void);
void BasCmdStop(void);
void BasCmdVar(void);
void BasCmdArray(void);
void BasCmdLet(void);
void BasCmdPrint(void);
void BasCmdInput(void);
void BasCmdCls(void);
void BasCmdPset(void);

s16  BasCalcCond(void);
s16  BasCalcExpr(void);
s16  BasCalcTerm(void);
s16  BasCalcFactor(void);
s16  BasCalcParam(void);
s16  BasCalcRnd(s16 val);

void BasSerialWriteChr(u8 c);
void BasSerialWriteStr(char* s);
void BasSerialWriteStrP(u16 s);
void BasSerialWriteNum(s16 val, s16 d);
void BasSerialWriteList(u8* p);
void BasSerialWriteNewLine(void);
u8   BasSerialRead(void);
bool BasSerialIsAvailable(void);

bool BasIsNoSpaceAfter(u8 code);
bool BasIsNoSpaceBefore(u8 code);
bool BasIsPrint(u8 c);
bool BasIsSpace(u8 c);
bool BasIsToupper(u8 c);
bool BasIsAlpha(u8 c);
bool BasIsDigit(u8 c);
bool BasIsString(u8 c);
bool BasIsCmd(u8* p);
bool BasIsRunList(void);
bool BasIsError(void);

void BasAddCode(u8 code);
void BasAddCode2(u8* p, s16 len);
u8*  BasAddCodeCmd(u8* p);
u8*  BasAddCodeNum(u8* p);
u8*  BasAddCodeStr(u8* p);
u8*  BasAddCodeVar(u8* p);

u8*  BasGetListPointer(s16 lineno);
s16  BasGetLineNo(u8* p);
u8   BasGetCmdIdx(u8* p);
u8   BasGetCmdLen(u8 idx);
u8*  BasSkipSpace(u8* p);


#ifdef __cplusplus
}
#endif
#endif
