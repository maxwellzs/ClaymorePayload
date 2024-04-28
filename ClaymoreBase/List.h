#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef void (*funcDestruct) (void* dat);
	typedef struct {
		char* key;
		size_t keyLength;
		void* value;
		funcDestruct f;
	} PairTraits;

	typedef struct ListNode {
		struct ListNode* next;
		struct ListNode* prev;
		void* dat;
		funcDestruct f;
	} ListNode;

	typedef struct {
		ListNode* first, * last;
		size_t size;
	} List;

#define PUSH_BACK(l,d) listPushBack(l,d,NULL);
#define PUSH_TRAIT(l,t) listPushBack(l,t,(funcDestruct) freeTraits);
#define GET_TRAIT(ln) ((PairTraits *)ln->dat)

	__declspec(dllexport) ListNode* createNode(void* dat, funcDestruct f);
	__declspec(dllexport) void freeNode(ListNode* n);
	__declspec(dllexport) List* createList();
	__declspec(dllexport) void listPushBack(List* l, void* dat, funcDestruct f);
	__declspec(dllexport) void listErase(List* l, ListNode* n);
	__declspec(dllexport) void listFree(List* l);
	__declspec(dllexport) ListNode* listPopFront(List* l);
	__declspec(dllexport) PairTraits* createTrait(const char* key,void * value, funcDestruct f);
	__declspec(dllexport) void freeTraits(PairTraits* t);

#ifdef __cplusplus
}
#endif