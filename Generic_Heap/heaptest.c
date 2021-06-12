# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "genvec.h"
# include "genHeap.h"

int Compare(const void *_left, const void *_right){
    if(*(int *)_left >= *(int *)_right){
        return 1;
    }
    else{
        return 0;
    }
}

void VecPrint(void *_item){
    if(_item != NULL ){
        printf("%d, ", *(int *)_item);
    }
}

int VectorFind(void* _element, size_t _index, void *_context){
    if(_element == NULL || _context == NULL){
        return -1;
    }
    if(*(int *)_element == *(int *)_context){
        return 0;
    }
    return 1;
}

int BuildTest(){
    Vector *vec;
    Heap *hp;
    int item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85;
    vec = VectorCreate(10, 10);
    VectorAppend(vec, &item1);
    VectorAppend(vec, &item2);
    VectorAppend(vec, &item3);
    VectorAppend(vec, &item4);
    VectorAppend(vec, &item5);
    VectorAppend(vec, &item6);
    VectorAppend(vec, &item7);
    VectorAppend(vec, &item8);
    hp = HeapBuild(vec, Compare);
    if(HeapSize(hp) != 8) 
        return 0;
    VectorPrint(vec, VecPrint);
    putchar('\n');
    return 1;
}


int InsertTest(){
    Vector *vec;
    Heap *hp;
    int i = 100, item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85;
    vec = VectorCreate(10, 10);
    VectorAppend(vec, &item1);
    VectorAppend(vec, &item2);
    VectorAppend(vec, &item3);
    VectorAppend(vec, &item4);
    VectorAppend(vec, &item5);
    VectorAppend(vec, &item6);
    VectorAppend(vec, &item7);
    VectorAppend(vec, &item8);
    hp = HeapBuild(vec, Compare);
    if(HeapInsert(NULL, Compare, &i) != HEAP_NOT_INITIALIZED)
        return 0;
    if(HeapInsert(hp, Compare, &i) != HEAP_SUCCESS)
        return 0;
    if(HeapSize(hp) != 9) 
        return 0;
    VectorPrint(vec, VecPrint);
    putchar('\n');
    return 1;
}

int PeekTest(){
    Vector *vec;
    Heap *hp;
    int i = 100, item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85;
    vec = VectorCreate(10, 10);
    VectorAppend(vec, &item1);
    VectorAppend(vec, &item2);
    VectorAppend(vec, &item3);
    VectorAppend(vec, &item4);
    VectorAppend(vec, &item5);
    VectorAppend(vec, &item6);
    VectorAppend(vec, &item7);
    VectorAppend(vec, &item8);
    hp = HeapBuild(vec, Compare);
    if(HeapInsert(NULL, Compare, &i) != HEAP_NOT_INITIALIZED)
        return 0;
    if(HeapInsert(hp, Compare, &i) != HEAP_SUCCESS)
        return 0;
    if(*(int *)HeapPeek(hp) != 100) 
        return 0;
    VectorPrint(vec, VecPrint);
    putchar('\n');
    return 1;
}

int ExtractTest(){
    Vector *vec;
    Heap *hp;
    int i = 100, item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85;
    vec = VectorCreate(10, 10);
    VectorAppend(vec, &item1);
    VectorAppend(vec, &item2);
    VectorAppend(vec, &item3);
    VectorAppend(vec, &item4);
    VectorAppend(vec, &item5);
    VectorAppend(vec, &item6);
    VectorAppend(vec, &item7);
    VectorAppend(vec, &item8);
    hp = HeapBuild(vec, Compare);
    if(HeapInsert(NULL, Compare, &i) != HEAP_NOT_INITIALIZED)
        return 0;
    if(HeapInsert(hp, Compare, &i) != HEAP_SUCCESS)
        return 0;
    if(*(int *)HeapExtract(hp, Compare) != 100){
        return 0;
    }
    if(*(int *)HeapPeek(hp) == 100) 
        return 0;
    if(HeapSize(hp) != 8)
        return 0;
    VectorPrint(vec, VecPrint);
    putchar('\n');
    return 1;
}

int ForEachTest(){
    Vector *vec;
    Heap *hp;
    int i = 100, item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85, test = 12;
    vec = VectorCreate(10, 10);
    VectorAppend(vec, &item1);
    VectorAppend(vec, &item2);
    VectorAppend(vec, &item3);
    VectorAppend(vec, &item4);
    VectorAppend(vec, &item5);
    VectorAppend(vec, &item6);
    VectorAppend(vec, &item7);
    VectorAppend(vec, &item8);
    hp = HeapBuild(vec, Compare);
    if(HeapForEach(hp, VectorFind, &test) >= 8){
        return 0;
    }
    return 1;
}



int main(void){
    if(BuildTest() == 1){
        printf("BuildTest - Succeeded\n");
    }
    else{
        printf("BuildTest - Faild\n");
    }
    if(InsertTest() == 1){
        printf("InsertTest - Succeeded\n");
    }
    else{
        printf("InsertTest - Faild\n");
    }
    if(PeekTest() == 1){
        printf("PeekTest - Succeeded\n");
    }
    else{
        printf("PeekTest - Faild\n");
    }
    if(ExtractTest() == 1){
        printf("ExtractTest - Succeeded\n");
    }
    else{
        printf("ExtractTest - Faild\n");
    }
    if(ForEachTest() == 1){
        printf("ForEachTest - Succeeded\n");
    }
    else{
        printf("ForEachTest - Faild\n");
    }
    
}