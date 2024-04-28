#pragma once
#include "List.h"

ListNode* createNode(void* dat, funcDestruct f)
{
	ListNode* n = (ListNode*)malloc(sizeof(ListNode));
	n->next = NULL;
	n->prev = NULL;
	n->dat = dat;
	n->f = f;
	return n;
}

void freeNode(ListNode* n)
{
	if (n != NULL && n->f != NULL) n->f(n->dat);
	free(n);
}

List* createList()
{
	List* l = (List*)malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

void listPushBack(List* l, void* dat, funcDestruct f)
{
	if (l->first == NULL) {
		ListNode* n = createNode(dat,f);
		l->first = l->last = n;
	}
	else {
		ListNode* last = l->last, * n = createNode(dat,f);
		last->next = n;
		n->prev = last;
		l->last = n;
	}
	l->size++;
}

void listErase(List* l, ListNode* n)
{
	if (l->first == n) {
		ListNode* nf = n->next;
		l->first = nf;
		if(nf) nf->prev = NULL;
		freeNode(n);
	}
	else if (l->last == n) {
		ListNode* nl = n->prev;
		l->last = nl;
		if (nl) nl->next = NULL;
		freeNode(n);
	}
	else {
		ListNode* np = n->prev, * nn = n->next;
		if(np) np->next = nn;
		if(nn) nn->prev = np;
		freeNode(n);
	}
	l->size--;
	if (l->size == 0) l->first = l->last = NULL;
}

void listFree(List* l)
{
	for (ListNode* n = l->first; n != NULL;) {
		ListNode* next = n->next;
		freeNode(n);
		n = next;
	}
}

ListNode* listPopFront(List* l)
{
	if (l->first == NULL) return NULL;
	ListNode* n = l->first;
	l->first = l->first->next;
	if(l->first != NULL) l->first->prev = NULL;
	l->size--;
	if (l->size == 0) 
		l->last = NULL;
	return n;
}

PairTraits* createTrait(const char* key, void* value, funcDestruct f)
{
	PairTraits* t = (PairTraits*)malloc(sizeof(PairTraits));
	t->f = f;
	t->key = (char*)malloc(strlen(key) + 1);
	memcpy(t->key, key, strlen(key) + 1);
	t->keyLength = strlen(key) + 1;
	t->value = value;
	return t;
}

void freeTraits(PairTraits* t)
{
	if (t != NULL && t->f != NULL) t->f(t->value);
	if (t) free(t->key);
	free(t);
}

