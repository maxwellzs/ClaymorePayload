// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#pragma comment(lib, "ClaymoreBase.lib")

BOOL
WINAPI
FakeCreateProcessW(
    _In_opt_ LPCWSTR lpApplicationName,
    _Inout_opt_ LPWSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCWSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOW lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
    MessageBoxW(NULL,lpCommandLine,L"warning",MB_OK);
    return false;
}

BOOL
WINAPI
FakeCreateProcessA(
    _In_opt_ LPCSTR lpApplicationName,
    _Inout_opt_ LPSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOA lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
    MessageBoxA(NULL, lpCommandLine, "warning", MB_OK);
    return false;
}

BOOL
WINAPI
FakeCreateProcessAsUserA(
    _In_opt_ HANDLE hToken,
    _In_opt_ LPCSTR lpApplicationName,
    _Inout_opt_ LPSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOA lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
    MessageBoxA(NULL, lpCommandLine, "warning", MB_OK);
    return false;
}

BOOL
WINAPI
FakeCreateProcessAsUserW(
    _In_opt_ HANDLE hToken,
    _In_opt_ LPCWSTR lpApplicationName,
    _Inout_opt_ LPWSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCWSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOW lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
    MessageBoxW(NULL, lpCommandLine, L"warning", MB_OK);
    return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    static BOOL initialized = false;
    JumpCode j1 = createJumpCode((LONG_PTR)FakeCreateProcessA),
        j2 = createJumpCode((LONG_PTR)FakeCreateProcessW),
        j3 = createJumpCode((LONG_PTR)FakeCreateProcessAsUserA),
        j4 = createJumpCode((LONG_PTR)FakeCreateProcessAsUserW);
    FARPROC realCreateProcessA = GetProcAddress(GetModuleHandleA("api-ms-win-core-processthreads-l1-1-0.dll"),"CreateProcessA"), 
        realCreateProcessW = GetProcAddress(GetModuleHandleA("api-ms-win-core-processthreads-l1-1-0.dll"), "CreateProcessW"),
        realCreateProcessAsUserW = GetProcAddress(GetModuleHandleA("api-ms-win-core-processthreads-l1-1-0.dll"), "CreateProcessAsUserW"),
        realCreateProcessAsUserA = GetProcAddress(GetModuleHandleA("api-ms-win-core-processthreads-l1-1-0.dll"), "CreateProcessAsUserA");
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!initialized) {
            MessageBoxA(NULL,"Injected","warning",MB_OK);
            ("CreateProcess -> 0x%p,0x%p\r\n", realCreateProcessA, realCreateProcessW);
            overwriteFunction(realCreateProcessA, j1);
            overwriteFunction(realCreateProcessW, j2);
            overwriteFunction(realCreateProcessAsUserA, j3);
            overwriteFunction(realCreateProcessAsUserW, j4);
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

