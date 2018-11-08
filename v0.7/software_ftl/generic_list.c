#include "generic_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

extern int memorySize;

sGenericList *listConstruct(unsigned int iHashSize){
    sListPtr newList;
	newList = (sGenericList*)malloc(sizeof(sGenericList));
    memorySize += 1;
	newList->head = (sElePtr) malloc (sizeof(sListElement));
    memorySize += 1;
	newList->tail = (sElePtr) malloc (sizeof(sListElement));
    memorySize += 1;

	newList->head->next = newList->tail;
	newList->head->prev = NULL;
	newList->tail->prev = newList->head;
	newList->tail->next = NULL;

	newList->iCurSize = 0;
	newList->iHashSize = iHashSize;

	if(iHashSize > 0){
		newList->hashTable = (sListHashPtr) malloc(sizeof(sGenericListIdx) * iHashSize);
        memorySize += 1;
		memset(newList->hashTable, 0x00, sizeof(sGenericListIdx) * iHashSize);
	}

	return newList;
}
void listDelete(sGenericList *li){
	if(li->iCurSize != 0){ // free elements
	}
    if(li->head != NULL){
	free(li->head);
    }

    if(li->tail != NULL){
	free(li->tail);
    }
    if(li->hashTable != NULL){
	free(li->hashTable);
    }
    if(li != NULL){
	free(li);
    }
    
}

sListElement *elementConstruct(unsigned long long int iKey, void* content){
	sListElement* newEle = (sListElement*)malloc(sizeof(sListElement));
	newEle->iKey = iKey;
	newEle->content = content;
	newEle->next = NULL;
	newEle->prev = NULL;

	return newEle;
}

void elementDelete(sListElement *ele){
	assert(ele != NULL);
	free(ele);
}

void listPushTail(sListPtr li, unsigned int iKey, void* content, unsigned int ch){
    sElePtr newEle;
    
    ACQUIRE_LOCK(ch);
    //barePrintf("6: ch = %d\n", ch);
	newEle = elementConstruct(iKey, content);
    RELEASE_LOCK(ch);

	newEle->prev = li->tail->prev;
	li->tail->prev = newEle;

	newEle->prev->next = newEle;
	newEle->next = li->tail;
	
	li->iCurSize++;	
}

void listPushHead(sListPtr li, unsigned int iKey, void* content){

	sElePtr newEle = elementConstruct(iKey, content);

	newEle->next = li->head->next;
	li->head->next = newEle;

	newEle->prev = li->head;
	li->head->prev = newEle;

	li->iCurSize++;
}

void listDeleteTail(sListPtr li){
	sElePtr target = li->tail->prev;
	assert(!isEmpty(li));

	li->tail->prev = target->prev;
	target->prev->next = li->tail;

	elementDelete(target);

	li->iCurSize--;
}

void listDeleteHead(sListPtr li){

	sElePtr target = li->head->next;	
	assert(!isEmpty(li));

	li->head->next = target->next;
	target->next->prev = li->head;

	elementDelete(target);

	li->iCurSize--;
}

void listDeleteEle(sListPtr li, sElePtr target){
	target->prev->next = target->next;
	target->next->prev = target->prev;
	elementDelete(target);
	li->iCurSize--;
}

void listSwapEle(sListPtr li, sElePtr ele_a, sElePtr ele_b){

	sListElement ele_a_cpy;

	memcpy(&ele_a_cpy, ele_a, sizeof(sListElement));	

	ele_a->content = ele_b->content;
	ele_a->iKey = ele_b->iKey;

	ele_b->content = ele_a_cpy.content;
	ele_b->iKey = ele_a_cpy.iKey;	
}

void listQuickSort(sListPtr li, int left, int right){

	unsigned int iSize = li->iCurSize;
	int i, j;
	unsigned long long int pivotVal = listGetNthEle(li, left)->iKey;
	
	if (left < right){

		i = left;	j = right + 1;

		while (i <= j){
			do{i++;} while (listGetNthEle(li, i)->iKey < pivotVal);
			do{j--;} while (listGetNthEle(li, j)->iKey > pivotVal);

			if (i < j){
				listSwapEle(li, listGetNthEle(li, i), listGetNthEle(li, j));
			}
			else{
				break;
			}
		}

		if (j > left){
			listSwapEle(li, listGetNthEle(li, left), listGetNthEle(li, j));
			listQuickSort(li, left, j - 1);
			listQuickSort(li, j + 1, right);
		}
		else{
			listQuickSort(li, left, j - 1);
			listQuickSort(li, j + 1, right);
		}
	}

	return;
}

void listEntireQuickSort(sListPtr li){
	listQuickSort(li, 0, li->iCurSize - 1);
	return;
}

sElePtr listFindKey(sListPtr li, unsigned int iKey){
	sElePtr ele = li->head->next;
	while(ele!= li->tail){		
		if(ele->iKey == iKey)
			return ele;
		else
			ele = ele->next;
	}

	
	//ftlAssert(0, "No proper iKey found.\n");

	return NULL;
}

sElePtr listGetHead(sListPtr li){
	return li->head->next;
}

sElePtr listGetTail(sListPtr li){
	return li->tail->prev;
}

void listInsertElement(sListPtr li, sElePtr newEle, sElePtr pos){
	newEle->next = pos->next;
	newEle->prev = pos->next->prev;
	pos->next = newEle;
	newEle->next->prev = newEle;

	li->iCurSize++;
}

void listInsertList(sListPtr li, sListPtr newList, sElePtr pos){
	sElePtr srcEle = newList->head->next;
	sElePtr newEle;
	sElePtr curPos = pos;

	while(srcEle != newList->tail)
	{
		newEle = elementConstruct(srcEle->iKey, srcEle->content);
		listInsertElement(li, newEle, pos);
		pos = pos->next;
		srcEle = srcEle->next;		
	}
}

sElePtr listGetNthEle(sListPtr li, unsigned int n){
	sElePtr ele = li->head->next;
	unsigned int i;

	assert(li->iCurSize >= n);

	for(i=0; i < n ; i++)
		ele = ele->next;

	return ele;
}

void* listGetNthContent(sListPtr li, unsigned int n){
	assert(li->iCurSize >= n);
	return listGetNthEle(li, n)->content;	
}

unsigned long long int listGetNthKey(sListPtr li, unsigned int n){
	assert(li->iCurSize >= n);
	return listGetNthEle(li, n)->iKey;	
}

unsigned int listGetIndex(sListPtr li, unsigned long long int iKey){
	unsigned int iRetVal;

	for(iRetVal = 0; iRetVal < li->iCurSize ; iRetVal++)
	{
		if(listGetNthKey(li, iRetVal) == iKey)
			return iRetVal;
	}
	return INVALID_IDX;
}

void listMoveToHead(sListPtr li, sElePtr ele){

	assert(li->iCurSize >= 1);
	
	if(ele == li->head->next)
		return;

	ele->next->prev = ele->prev;
	ele->prev->next = ele->next;

	ele->next = li->head->next;
	ele->prev = li->head;

	li->head->next->prev = ele;
	li->head->next = ele;
}

void listMoveToTail(sListPtr li, sElePtr ele){

	assert(li->iCurSize >= 1);

	if(ele->next == li->tail)
		return;

	ele->next->prev = ele->prev;
	ele->prev->next = ele->next;

	ele->prev = li->tail->prev;
	ele->next = li->tail;

	li->tail->prev->next = ele;
	li->tail->prev = ele;
}

void listMoveToNthPos(sListPtr li, sElePtr ele, unsigned int n){

	sElePtr newElement = (sElePtr)malloc(sizeof(sListElement));
	sElePtr nthEle = listGetNthEle(li, n);
	unsigned int i;
		
    memorySize += 1;
	memcpy(newElement, ele, sizeof(sListElement));

	listDeleteEle(li, ele);

	newElement->next = nthEle->next;
	newElement->prev = nthEle->next->prev;

	nthEle->next = newElement;
	newElement->next->prev = newElement;

	return;	
}

char isEmpty(sListPtr li){
	return (li->iCurSize == 0) ? 1 : 0;
}

void printList(sListPtr li){
	
	sElePtr ele = li->head->next;
	
	printf("Size:%d::::", li->iCurSize);
	printf("Head->");
	while(ele != li->tail){
		printf("%d/%d->", ele->iKey, (unsigned int)ele->content);
		ele = ele->next;
	}
	printf("Tail\n");
}
