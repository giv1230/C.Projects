# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "hashset.h"

# define MAGIC 248968

int **IntArray(){
    int **arr, i;
    arr = (int **)malloc(sizeof(int *) * 10);
    for (i = 0; i < 10; i++){
        arr[i] = (int *)malloc(sizeof(int));
        *arr[i] = i;
    }
    for(i = 0; i < 10; i++){
        printf("%d, ", *arr[i]);
    }
    putchar('\n');
    return arr;
}

int Equality(void *_left, void *_right){
    if(*(int *)_left == *(int *)_right){
        return 1;
    }
    else{
        return 0;
    }
}

size_t HashFunc(void* _element){
    int elm = *(int *)_element;
    return elm;
}

void Destroyer(void* _element){
    free(_element);
}

void PrintFunction(void* _element){
    int elm = *(int *)_element;
    printf("%d, ", elm);
}

/* Check if it works */
int CreateTest1(){
    Hash *hash;
    hash = HashCreate(10, HashFunc, Equality);
    if(NULL == hash){
        return 0;
    }
    if(hash->m_hashSize != 10){
        return 0;
    }
    if(hash->m_numOfItems != 0){
        return 0;
    }
    if(hash->m_maxOfRehashOperations != 0){
        return 0;
    }
    if(hash->m_counterOfInsertions != 0){
        return 0;
    }
    if(hash->m_allRehashOperations != 0){
        return 0;
    }
    if(hash->m_magic != MAGIC){
        return 0;
    }
    HashDestroy(&hash, Destroyer);
    return 1;
}


/* Check TreeInsert function */
int InsertTest1(){
    Hash *hash;
    int i, j, **arr;
    int *temp;
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp[0] = j;
    hash = HashCreate(10, HashFunc, Equality);
    arr = IntArray();
    if(HashInsert(NULL, arr[0]) != SET_UNINITIALIZED)
        return 0;
    for(i = 0; i < 10; i++){
        if(HashInsert(hash, arr[i]) != SET_SUCCESS)
            return 0;
        if(i < 9 && HashInsert(hash, arr[i]) != SET_KEY_DUPLICATE)
            return 0;
    }
    if(HashInsert(hash, temp) != SET_OVERFLOW)
        return 0;
    if(hash->m_numOfItems != 10)
        return 0;
    if(hash->m_counterOfInsertions != 10)
        return 0;
    if(hash->m_maxOfRehashOperations != 0)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

/* Check TreeIsDataFound() function */
int RemoveTest1(){
    Hash *hash;
    void *temp1;
    int i, j, *temp, **arr = IntArray();
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp[0] = j;
    temp1 = &j;
    hash = HashCreate(10, HashFunc, Equality);
    if(HashRemove(hash, arr[8], &temp1) != SET_UNDERFLOW)
        return 0;
    for(i = 0; i < 10; i++){
        HashInsert(hash, arr[i]);
    }
    if(HashRemove(NULL, arr[8], &temp1) != SET_UNINITIALIZED)
        return 0;
    if(HashRemove(hash, arr[8], &temp1) != SET_SUCCESS)
        return 0;
    if(HashRemove(hash, arr[8], &temp1) != SET_KEY_NOT_FOUND)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

int HashFoundTest(){
    Hash *hash;
    int i, j, **arr = IntArray();
    void *temp, *temp1;
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp = &j;
    temp1 = &j;
    hash = HashCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashInsert(hash, arr[i]);
    }
    if(HashIsFound(NULL, arr[8]) != 0)
        return 0;
    if(HashIsFound(hash, arr[8]) != 1)
        return 0;
    if(HashRemove(hash, arr[8], temp1) != SET_SUCCESS)
        return 0;
    if(HashIsFound(hash, arr[8]) != 0)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

int HashNumOfItemsTest(){
    Hash *hash;
    int i, j, *temp, **temp1, **arr = IntArray();
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp[0] = j;
    hash = HashCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashInsert(hash, arr[i]);
    }
    if(HashNumOfItems(NULL) != 0)
        return 0;
    if(HashNumOfItems(hash) != 10)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

int HashCapacityTest(){
    Hash *hash;
    int i, j;
    hash = HashCreate(10, HashFunc, Equality);
    if(HashCapacity(NULL) != 0)
        return 0;
    if(HashCapacity(hash) != 10)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

int MaxReHashTest(){
    Hash *hash;
    int i, **arr = IntArray();
    hash = HashCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashInsert(hash, arr[i]);
    }
    if(HashMaxReHash(NULL) != 0)
        return 0;
    if(HashMaxReHash(hash) != 0)
        return 0;
    HashDestroy(&hash, Destroyer);
    return 1;
}

/* Check TreePrint() function */
void PrintTest1(){
    Hash *hash;
    int i, **arr = IntArray();
    hash = HashCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashInsert(hash, arr[i]);
    }
    HashPrint(hash, PrintFunction);
}

int main(void){
    int i;
    /* CreateTest */
    i = CreateTest1();
    if(i == 1){
        printf("CreateTest1 = OK\n");
    }
    else{
        printf("CreateTest1 = FAIL\n");
    }

    /* InsertTest1 */
    i = InsertTest1();
    if(i == 1){
        printf("InsertTest1 = OK\n");
    }
    else{
        printf("InsertTest1 = FAIL\n");
    }

    i = RemoveTest1();
    if(i == 1){
        printf("RemoveTest1 = OK\n");
    }
    else{
        printf("RemoveTest1 = FAIL\n");
    }

    i = HashFoundTest();
    if(i == 1){
        printf("HashFoundTest = OK\n");
    }
    else{
        printf("HashFoundTest = FAIL\n");
    }

    i = HashNumOfItemsTest();
    if(i == 1){
        printf("HashNumOfItemsTest = OK\n");
    }
    else{
        printf("HashNumOfItemsTest = FAIL\n");
    }

    i = HashCapacityTest();
    if(i == 1){
        printf("HashCapacityTest = OK\n");
    }
    else{
        printf("HashCapacityTest = FAIL\n");
    }

    i = MaxReHashTest();
    if(i == 1){
        printf("MaxReHashTest = OK\n");
    }
    else{
        printf("MaxReHashTest = FAIL\n");
    }

    PrintTest1();
}


/*
    int  *i1, *i2, *i3, *i4, *i5, *i6, *i7, *i8, *i9, *i10;
    i1 = (int *)malloc(sizeof(int));
    i2 = (int *)malloc(sizeof(int));
    i3 = (int *)malloc(sizeof(int));
    i4 = (int *)malloc(sizeof(int));
    i5 = (int *)malloc(sizeof(int));
    i6 = (int *)malloc(sizeof(int));
    i7 = (int *)malloc(sizeof(int));
    i8 = (int *)malloc(sizeof(int));
    i9 = (int *)malloc(sizeof(int));
    i10 = (int *)malloc(sizeof(int));
    i1[0] = 1;
    i2[0] = 2;
    i3[0] = 3;
    i4[0] = 4;
    i5[0] = 5;
    i6[0] = 6;
    i7[0] = 7;
    i8[0] = 8;
    i9[0] = 9;
    i10[0] = 10;
    arr[0] = i1;
    arr[1] = i2;
    arr[2] = i3;
    arr[3] = i4;
    arr[4] = i5;
    arr[5] = i6;
    arr[6] = i7;
    arr[7] = i8;
    arr[8] = i9;
    arr[9] = i10;
    */