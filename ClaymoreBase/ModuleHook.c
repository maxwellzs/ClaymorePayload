#include "pch.h"
#include "ModuleHook.h"

JumpCode createJumpCode(LONG_PTR func)
{
	static JumpCode j = {0x68,0,{0xC7,0x44,0x24 ,0x04},0,0xC3};
	j.h4 = func >> (4 * 8);
	j.l4 = func & 0xffffffff;
	return j;
}

int overwriteFunction(FARPROC original, JumpCode code)
{
	if (original == NULL) return 0;
	static DWORD writeSize = sizeof(JumpCode);
	DWORD oldProtect;
	int ret = VirtualProtect(original, writeSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	if (!ret) return 0;
	memcpy(original, &code, writeSize);
	VirtualProtect(original, writeSize, oldProtect, &oldProtect);
	return 1;
}
