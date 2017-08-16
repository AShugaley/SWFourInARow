#include "SPArrayList.h"
#include <stdio.h>
#include <stdlib.h>


SPArrayList* spArrayListCreate(int maxSize)
{
    if (maxSize <= 0)
        return NULL;
    
    SPArrayList * arrLstForUndo = (SPArrayList *)malloc(sizeof(SPArrayList));
    if (arrLstForUndo == NULL)
        return NULL;
    
    arrLstForUndo->elements = (int*)malloc(maxSize * sizeof(int));
    arrLstForUndo->maxSize = maxSize;
    arrLstForUndo->actualSize = 0;
    return arrLstForUndo;
}


SPArrayList* spArrayListCopy(SPArrayList* src)
{
    int i = 0;
    if (src == NULL)
        return NULL;
    
    SPArrayList * arrLstCopy = spArrayListCreate(src->maxSize);
    
    arrLstCopy->actualSize = src->actualSize;
    for (i = 0; i < arrLstCopy->actualSize; i++)
    {
        (arrLstCopy->elements)[i] = (src->elements)[i];
    }
    return arrLstCopy;
    
}

void spArrayListDestroy(SPArrayList* src)
{
    if (src != NULL)
    {
        free(src->elements);	//free to int*
        free(src);
    }
}


SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src)
{
    if (src == NULL)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    else
    {
        src->actualSize = 0;
    }
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index)
{
    int i = 0;
    if (src == NULL || index > src->actualSize || index < 0)	//if index is out of ACTUAL size- will create discontinuity of the memory
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsFull(src))
        return SP_ARRAY_LIST_FULL;
    
    for (i = src->actualSize; i > index; i--)
    {
        src->elements[i] = src->elements[i - 1];
    }
    src->elements[index] = elem;
    src->actualSize++;
    
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem)
{
    int i = 0;
    if (src == NULL)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsFull(src))
        return SP_ARRAY_LIST_FULL;
    
    for (i = src->actualSize; i > 0; i--)
    {
        src->elements[i] = (src->elements)[i - 1];
    }
    
    src->elements[0] = elem;
    //printf("elements - %d\n", src->elements[0]);
    src->actualSize++;
    
    return SP_ARRAY_LIST_SUCCESS;
    
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem)
{
    if (src == NULL)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsFull(src))
        return SP_ARRAY_LIST_FULL;
    
    src->elements[src->actualSize] = elem;
    src->actualSize++;
    
    return SP_ARRAY_LIST_SUCCESS;
    
    
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index)
{
    int i = 0;
    if (src == NULL || index >= src->actualSize || index < 0)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsEmpty(src))
        return SP_ARRAY_LIST_EMPTY;
    
    for (i = index; i<src->actualSize - 1; i++)
    {
        src->elements[i] = src->elements[i + 1];
    }
    
    src->actualSize--;
    
    return SP_ARRAY_LIST_SUCCESS;
    
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src)
{
    int i = 0;
    if (src == NULL)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsEmpty(src))
        return SP_ARRAY_LIST_EMPTY;
    
    for (i = 0; i<src->actualSize - 1; i++)
    {
        src->elements[i] = src->elements[i + 1];
    }
    
    src->actualSize--;
    
    return SP_ARRAY_LIST_SUCCESS;
    
    
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src)
{
    if (src == NULL)
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    
    if (spArrayListIsEmpty(src))
        return SP_ARRAY_LIST_EMPTY;
    
    (src->actualSize)--;
    
    return SP_ARRAY_LIST_SUCCESS;
    
}

int spArrayListGetAt(SPArrayList* src, int index)
{
    if (src == NULL || index >= src->actualSize || index < 0)
        return -1;
    return src->elements[index];
    
}

int spArrayListGetFirst(SPArrayList* src)
{
    if (src == NULL || src->actualSize == 0)
        return -1;
    return src->elements[0];
}

int spArrayListGetLast(SPArrayList* src)
{
    if (src == NULL || src->actualSize == 0)
        return -1;
    return src->elements[src->actualSize - 1];
    
}

int spArrayListMaxCapacity(SPArrayList* src)
{
    if (src == NULL)
        return -1;
    return src->maxSize;
}

int spArrayListSize(SPArrayList* src)
{
    if (src == NULL)
        return -1;
    return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src)
{
    if ((src->maxSize > src->actualSize) || (src == NULL))
        return false;
    return true;
}

bool spArrayListIsEmpty(SPArrayList* src)
{
    if ((src->actualSize != 0) || (src == NULL))
        return false;
    return true;
    
}
