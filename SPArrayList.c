#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SPArrayList.h"

/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int maxSize){
	SPArrayList* createdArrayList;
	if (maxSize<=0)
		return NULL;
	createdArrayList = (SPArrayList*) malloc(sizeof(SPArrayList));
	if( createdArrayList == NULL)
		return NULL;
	createdArrayList->actualSize = 0;
	createdArrayList->elements = (HistoryElement**)calloc(sizeof(HistoryElement*),maxSize);
	createdArrayList->maxSize = maxSize;
	return createdArrayList;
}


SPArrayList* spArrayListCopy(SPArrayList* src){
	int i;
	SPArrayList* copyArrayList = spArrayListCreate(src->maxSize);
	for(i=0; i<src->actualSize; i++)
		copyArrayList->elements[i]=CopyMoveHistory(src->elements[i]);
	copyArrayList->actualSize=src->actualSize;
	return copyArrayList;
}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void spArrayListDestroy(SPArrayList* src){
	int i;
	if(src==NULL)
		return;
	if(src->elements!=NULL){
		for(i=0; i<src->maxSize; i++)
			DestroyMoveHistory(src->elements[i]);
		free(src->elements);
	}
	free(src);
}

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * SP_ARRAY_LIST_SUCCESS otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){ ////DEBUG: apparently not used
	int listSize=src->actualSize, i;
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	else {
		for(i=0; i<listSize; i++)
			spArrayListRemoveFirst(src);
	}
	return SP_ARRAY_LIST_SUCCESS;
}

void printArrayList(SPArrayList* list){
	int i;
	printf("[");
	for(i=0; i<list->actualSize; i++){
		PrintHistoryMove(list->elements[i]);
	}
	printf("] actual=%d\n", list->actualSize);
	fflush(stdout);
}
/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, HistoryElement *elem, int index){
	////DEBUG check if used
	int i;
	if (src == NULL || index >= src->maxSize)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == src->maxSize)
		return SP_ARRAY_LIST_FULL;

	for (i=src->actualSize; i>index; i--) {
		src->elements[i] = src->elements[i-1];
	}
	src->elements[index] = elem;
	src->actualSize++;
	return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, HistoryElement *elem){
		return spArrayListAddAt(src, elem, 0);
 }

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, HistoryElement *elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @param index - The index from where the new element will be removed.
 *                The index is 0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	int i;
	if (src == NULL || index >= src->actualSize)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == 0)
		return SP_ARRAY_LIST_EMPTY;

	DestroyMoveHistory(src->elements[index]);
	for (i=index; i<src->actualSize; i++) {
		src->elements[i] = src->elements[i+1];
	}
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise.
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, src->actualSize-1);
}

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
HistoryElement * spArrayListGetAt(SPArrayList* src, int index){
	return src->elements[index];
}

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
HistoryElement * spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);
}

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
HistoryElement * spArrayListGetLast(SPArrayList* src){
	return spArrayListGetAt(src, src->actualSize-1);
}

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int spArrayListSize(SPArrayList* src){
	return src->actualSize;
}

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool spArrayListIsFull(SPArrayList* src){
	if (src->actualSize == src->maxSize)
		return true;
	return false;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src){
	if (src->actualSize == 0)
		return true;
	return false;
}

