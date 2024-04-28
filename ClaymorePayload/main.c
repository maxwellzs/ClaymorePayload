#include <stdio.h>
#include <Windows.h>
#include "List.h"
#include "Process.h"
#include "ModuleHook.h"

int main(int argc, char** argv) {
	List* l = createListOfProcess();
	Process* p = getProcessByName(l,L"explorer.exe");
	if (p != NULL) {
		openProcess(p);
		if (p->opened) {
			printf("successfully opened\r\n");
		}
		HANDLE h = injectIntoProcess(p,L"C:\\Users\\maxwellzs\\source\\repos\\ClaymorePayload\\x64\\Debug\\ClaymoreInjection.dll");
		WaitForSingleObject(h, INFINITE);
		DWORD exitCode;
		GetExitCodeThread(h, &exitCode);
		printf("remote thread exited with %d\r\n", exitCode);
	}
	printf("exited\r\n");
	system("pause");
	return 0;
}