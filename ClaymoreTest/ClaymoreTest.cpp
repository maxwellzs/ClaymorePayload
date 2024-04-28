#include "pch.h"
#include "CppUnitTest.h"
#include "List.h"
#include "Process.h"
#include "ModuleHook.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

int fake_printf(
	char const* const _Format,
	...) {
	char c[] = 
		" _   _ ___    _   _    ____ _  _______ ____  _	\n"
		"| | | |_ _|  | | / \  / ___| |/ / ____|  _ \| |\n"
		"| |_| || |_  | |/ _ \| |   | ' /|  _| | | | | |\n"
		"|  _  || | |_| / ___ \ |___| . \| |___| |_| |_|\n"
		"|_| |_|___\___/_/   \_\____|_|\_\_____|____/(_)\n"
		;
	MessageBoxA(NULL,c,"¾¯¸æ",MB_OK);
	return 0;
}

namespace ClaymoreTest
{
	TEST_CLASS(ClaymoreTest)
	{
	public:
		
		TEST_METHOD(TestLinkedListPush)
		{
			List* l = createList();
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "efgh");
			PUSH_BACK(l, "hijk");
			PUSH_BACK(l, "abcd");
			for (ListNode* n = l->first; n != NULL; n = n->next) {
				printf("%s\n", (char*)n->dat);
			}
			listFree(l);
		}

		TEST_METHOD(TestLinkedListRemove)
		{
			List* l = createList();
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "efgh");
			PUSH_BACK(l, "hijk");
			PUSH_BACK(l, "lmno");
			PUSH_BACK(l, "pqrs");
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "abcd");

			listErase(l,l->first);
			listErase(l,l->first);
			listErase(l,l->last);
			for (ListNode* n = l->first; n != NULL; n = n->next) {
				Logger::WriteMessage((char *)n->dat);
				Logger::WriteMessage("\r\n");
			}
			listFree(l);
		}

		TEST_METHOD(TestLinkedListPopFront) {
			List* l = createList();
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "efgh");
			PUSH_BACK(l, "hijk");
			PUSH_BACK(l, "lmno");
			PUSH_BACK(l, "pqrs");
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "abcd");
			PUSH_BACK(l, "abcd");
			while (l->size > 2) {
				ListNode* n = listPopFront(l);
				Logger::WriteMessage((char*) n->dat);
				Logger::WriteMessage("\r\n");
				freeNode(n);
			}
			listErase(l, l->first);
			listErase(l, l->first);
			listFree(l);
		}

		TEST_METHOD(TestPairTraits) {
			List* l = createList();
			PairTraits* t1 = createTrait("key1",NULL,NULL),
				* t2 = createTrait("key2", NULL, NULL),
				* t3 = createTrait("key3", NULL, NULL),
				* t4 = createTrait("key4",NULL,NULL);
			PUSH_TRAIT(l,t1);
			PUSH_TRAIT(l,t2);
			PUSH_TRAIT(l,t3);
			PUSH_TRAIT(l,t4);
			listErase(l,l->first);
			listErase(l,l->last);
			listErase(l,l->last);
			Logger::WriteMessage("remain key :");
			Logger::WriteMessage(GET_TRAIT(l->first)->key);
			listFree(l);
		}

		TEST_METHOD(TestEnumProcess) {
			Logger::WriteMessage("start to dump system process.\r\n");
			List* l = createListOfProcess();
			Assert::IsNotNull(l);
			Logger::WriteMessage(("process amount " + std::to_string(l->size) + " \r\n").c_str());
			for (ListNode* n = l->first; n != NULL; n = n->next) {
				Process* p = (Process*)n->dat;
				std::wstring str = L"pid=" + std::to_wstring(p->pid) + L",parent=" + std::to_wstring(p->parent) + L",path=" + std::wstring(p->exe_path) + L"\r\n";
				Logger::WriteMessage(str.c_str());
			}
			listFree(l);
		}

		TEST_METHOD(TestQueryProcess) {
			List* l = createListOfProcess();
			Assert::IsNotNull(l);
			Process* p = getProcessByName(l,L"[System Process]");
			Assert::IsNotNull(p);
			p = getProcessByName(l,L"NotExist.exe");
			Assert::IsNull(p);
			p = getProcessByName(l,L"clash-core-service.exe");
			if (p) {
				std::string msg = "clash is running at pid " + std::to_string(p->pid) + "\r\n";
				Logger::WriteMessage(msg.c_str());
			}
			listFree(l);
		}

		TEST_METHOD(TestQueryChildByName) {
			List* l = createListOfProcess(), * l2;
			Assert::IsNotNull(l);
			l2 = listChildrenProcessByName(l,L"service.exe");
			Assert::IsNotNull(l2);
			for (ListNode* n = l2->first; n != NULL; n = n->next) {
				Process* p = (Process*)n->dat;
				std::wstring str = L"pid=" + std::to_wstring(p->pid) + L",path=" + std::wstring(p->exe_path) + L"\r\n";
				Logger::WriteMessage(str.c_str());
			}
			listFree(l);
			listFree(l2);
		}

		TEST_METHOD(TestQueryChildByID) {
			List* l = createListOfProcess(), * l2;
			Assert::IsNotNull(l);
			Process* p = getProcessByName(l,L"explorer.exe");
			Assert::IsNotNull(p);
			std::string msg = "explorer is at " + std::to_string(p->pid) + "\r\n";
			Logger::WriteMessage(msg.c_str());
			l2 = listChildrenProcessByPid(l,p->pid);
			Assert::IsNotNull(l2);
			for (ListNode* n = l2->first; n != NULL; n = n->next) {
				Process* p = (Process*)n->dat;
				std::wstring str = L"pid=" + std::to_wstring(p->pid) + L",path=" + std::wstring(p->exe_path) + L"\r\n";
				Logger::WriteMessage(str.c_str());
			}
			listFree(l);
			listFree(l2);
		}

		TEST_METHOD(TestCreateRemoteThread) {
			List* l = createListOfProcess();
			Assert::IsNotNull(l);

			Process* p = getProcessByName(l, L"explorer.exe");
			std::string DLLPath = "C:\\Users\\maxwellzs\\source\\repos\\ClaymorePayload\\x64\\Debug\\ClaymoreInjection.dll";
			HANDLE Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, p->pid);

			// We get the address of the LoadLibrary instruction in the target process
			LPVOID LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

			// We allocate a memory space in this process to store the name of the DLL that it will load
			LPVOID Memory = (LPVOID)VirtualAllocEx(Process, NULL, DLLPath.length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);


			// We write in this memory space the name of the DLL to inject
			WriteProcessMemory(Process, (LPVOID)Memory, DLLPath.c_str(), DLLPath.length() + 1, NULL);

			// And finally we create a thread running in the context of the target process

			// This thread will only execute a LoadLibrary instruction with the path of the DLL to inject as argument.

			HANDLE RemoteThread = CreateRemoteThread(Process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);
			DWORD exit;
			WaitForSingleObject(RemoteThread, INFINITE);
			GetExitCodeThread(RemoteThread, &exit);
			Logger::WriteMessage(std::to_string(exit).c_str());
		}
	
		TEST_METHOD(TestInlineHookExample) {
			DWORD oldProtect;
#pragma pack(1)
			struct INSTRUCT {
				const BYTE PUSH = 0x68;
				INT32 l4 = 0;
				const BYTE MOV[4] = { 0xC7 ,0x44 , 0x24 ,0x04 };
				INT32 h4 = 0;
				const BYTE RET = 0xC3;
			};
#pragma pack()
			
			INSTRUCT i;
			i.h4 = (INT32)((INT64)fake_printf >> 32);
			i.l4 = (INT64)fake_printf;
			char* c = (char*)&i;
			Assert::IsTrue(VirtualProtect(printf,sizeof(INSTRUCT),PAGE_EXECUTE_READWRITE,&oldProtect));
			memcpy(printf, c, sizeof(i));
			printf("");
		}

		TEST_METHOD(TestDllInject) {
			List* l = createListOfProcess();
			Process* p = getProcessByName(l, L"jshell.exe");
			Assert::IsNotNull(p);
			Assert::IsTrue(openProcess(p));
			injectIntoProcess(p, L"C:\\Users\\maxwellzs\\source\\repos\\ClaymorePayload\\x64\\Debug\\ClaymoreBase.dll");
		}

		TEST_METHOD(TestFunctionOverwrite) {
			
		}
	};
}
