#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
	typedef struct {
		BYTE PUSH;
		INT32 l4;
		BYTE MOV[4];
		INT32 h4;
		BYTE RETN;
	} JumpCode;
#pragma pack()

	__declspec(dllexport) JumpCode createJumpCode(LONG_PTR func);
	__declspec(dllexport) int overwriteFunction(FARPROC original, JumpCode code);

#ifdef __cplusplus
}
#endif