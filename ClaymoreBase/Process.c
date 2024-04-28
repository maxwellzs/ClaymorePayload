#include "Process.h"

Process* recordProcess(DWORD pid, DWORD parent, wchar_t* path)
{
    Process* p = (Process*)malloc(sizeof(Process));
    p->pid = pid;
    p->parent = parent;
    size_t copySize = (wcslen(path) + 1) * sizeof(wchar_t);
    p->exe_path = (char*)malloc(copySize);
    memcpy(p->exe_path, path, copySize);
    p->opened = 0;
    return p;
}

void releaseProcessRecord(Process* p)
{
    if(p) free(p->exe_path);
    if (p && p->opened) CloseHandle(p->procHandle);
    free(p);
}

List* createListOfProcess()
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        printf("error when creating process snapshot, error code : %d\n", GetLastError());
        return NULL;
    }
    PROCESSENTRY32 entry = {0};
    entry.dwSize = sizeof(PROCESSENTRY32);
    BOOL ret = Process32First(snapshot,&entry);
    List* l = createList();
    while (ret) {
        listPushBack(l,recordProcess(entry.th32ProcessID, entry.th32ParentProcessID, entry.szExeFile), releaseProcessRecord);
        ret = Process32Next(snapshot,&entry);
    }
    CloseHandle(snapshot);
    return l;
}

Process* getProcessByName(List* processList, const wchar_t* name)
{
    for (ListNode* n = processList->first; n != NULL; n = n->next) {
        Process* p = n->dat;
        if (!wcscmp(p->exe_path, name)) return p;
    }
    return NULL;
}

List* listChildrenProcessByName(List* processList, const wchar_t* parent)
{
    List* l = createList();
    DWORD parentID = -1;
    for (ListNode* n = processList->first; n != NULL; n = n->next) {
        Process* p = n->dat;
        if (!wcscmp(p->exe_path, parent)) {
            parentID = p->pid;
        }
    }
    if (parentID == -1) return l;
    for (ListNode* n = processList->first; n != NULL; n = n->next) {
        Process* p = n->dat;
        if (p->parent == parentID) listPushBack(l,recordProcess(p->pid,p->parent,p->exe_path),releaseProcessRecord);
    }
    return l;
}

List* listChildrenProcessByPid(List* processList, DWORD pid)
{
    List* l = createList();
    for (ListNode* n = processList->first; n != NULL; n = n->next) {
        Process* p = n->dat;
        if (p->parent == pid) listPushBack(l, recordProcess(p->pid, p->parent, p->exe_path), releaseProcessRecord);
    }
    return l;
}

int openProcess(Process* proc)
{
    DWORD pid = proc->pid;
    proc->procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (proc->procHandle != NULL) {
        proc->opened = 1;
        return 1;
    }   
    return 0;
}

HANDLE injectIntoProcess(Process* proc, const wchar_t* dllPath)
{
    LPVOID pLibRemote = NULL;
    DWORD hLibModule;
    HMODULE hModule = GetModuleHandleW(L"kernel32.dll");
    DWORD writeSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    pLibRemote = VirtualAllocEx(
        proc->procHandle,
        NULL,
        writeSize,
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
    );
    WriteProcessMemory(proc->procHandle, pLibRemote, dllPath, writeSize, NULL);
    LPTHREAD_START_ROUTINE dwLoadAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryW");
    HANDLE hThread = CreateRemoteThread(
        proc->procHandle,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)dwLoadAddr,
        pLibRemote,
        NULL,
        NULL
    );
    return hThread;
}
