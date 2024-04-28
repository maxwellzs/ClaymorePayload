#pragma once

#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include "List.h"
#include "Def.h"

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct {
		DWORD pid;
		DWORD parent;
		wchar_t* exe_path;
		HANDLE procHandle;
		int opened;
	} Process;

	/// <summary>
	/// create a column of process in the list
	/// </summary>
	/// <param name="pid">the pid of the process</param>
	/// <param name="path">the name of the process, wchar</param>
	/// <returns>the created process handler</returns>
	__declspec(dllexport) Process* recordProcess(DWORD pid,DWORD parent,wchar_t* path);
	__declspec(dllexport) void releaseProcessRecord(Process* p);
	/// <summary>
	/// enum the list of process in this system
	/// </summary>
	/// <returns></returns>
	__declspec(dllexport) List* createListOfProcess();
	/// <summary>
	/// query a process by its name, if no process is found, then return NULL
	/// </summary>
	/// <param name="processList">the list containing the parameters</param>
	/// <param name="name">the given name</param>
	/// <returns>the pointer to the found process</returns>
	__declspec(dllexport) Process* getProcessByName(List* processList, const wchar_t* name);
	__declspec(dllexport) List* listChildrenProcessByName(List* processList, const wchar_t* parent);
	__declspec(dllexport) List* listChildrenProcessByPid(List* processList, DWORD pid);
	__declspec(dllexport) int openProcess(Process* proc);
	__declspec(dllexport) HANDLE injectIntoProcess(Process* proc,const wchar_t* dllPath);

#ifdef __cplusplus
}
#endif