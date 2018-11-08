#ifndef __GENERIC_LIST_H__
#define __GENERIC_LIST_H__

typedef struct genericListElement{
	
	struct genericListElement *next;
	struct genericListElement *prev;
	unsigned long long int iKey;
	void *content;

}sListElement;

typedef struct genericListHash{
	sListElement *next;
	sListElement *target;
}sGenericListIdx;

typedef struct genericList{

	sListElement *head;
	sListElement *tail;
	unsigned int iCurSize;	
	unsigned int iHashSize;
	sGenericListIdx *hashTable;
	
}sGenericList;

#define INVALID_IDX		0xffffffff

typedef sGenericList * sListPtr;
typedef sListElement * sElePtr;
typedef sGenericListIdx * sListHashPtr;

sGenericList *listConstruct(unsigned int iHashSize);
void listDelete(sGenericList *li);

sListElement *elementConstruct(unsigned long long int iKey, void* content);
void elementDelete(sListElement *ele);

void listPushTail(sListPtr li, unsigned int iKey, void* content, unsigned int ch);
void listPushHead(sListPtr li, unsigned int iKey, void* content);

void listDeleteTail(sListPtr li);
void listDeleteHead(sListPtr li);
void listDeleteEle(sListPtr li, sElePtr target);

sElePtr listFindKey(sListPtr li, unsigned int iKey);
sElePtr listGetHead(sListPtr li);
sElePtr listGetTail(sListPtr li);

void listInsertElement(sListPtr li, sElePtr newEle, sElePtr pos);
void listInsertList(sListPtr li, sListPtr newList, sElePtr pos);

sElePtr listGetNthEle(sListPtr li, unsigned int n);
void* listGetNthContent(sListPtr li, unsigned int n);
unsigned long long int listGetNthKey(sListPtr li, unsigned int n);

unsigned int listGetIndex(sListPtr li, unsigned long long int iKey);

void listMoveToHead(sListPtr li, sElePtr ele);
void listMoveToTail(sListPtr li, sElePtr ele);
void listMoveToNthPos(sListPtr li, sElePtr ele, unsigned int n);
char isEmpty(sListPtr li);

void listSwapEle(sListPtr li, sElePtr ele_a, sElePtr ele_b);
void listQuickSort(sListPtr li,int left, int right);
void listEntireQuickSort(sListPtr li);

void printList(sListPtr li);

#endif
