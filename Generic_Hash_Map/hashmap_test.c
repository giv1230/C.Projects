# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "HashMap.h"
# include "list.h"
# include "list_itr.h"

# define MAGIC 248968


int **IntArray(){
    int **arr, i;
    arr = (int **)malloc(sizeof(int *) * 10);
    for (i = 0; i < 10; i++){
        arr[i] = (int *)malloc(sizeof(int));
        *arr[i] = i;
    }
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
    int *dest = (int*)_element;
    free(dest);
}

void PrintFunction(void* _element){
    int elm = *(int *)_element;
    printf("%d, ", elm);
}

/* Check if it works */
int CreateTest1(){
    HashMap *hash;
    if(HashMapCreate(0, HashFunc, Equality) != NULL)
        return 0;
    if(HashMapCreate(10, NULL, Equality) != NULL)
        return 0;
    if(HashMapCreate(10, HashFunc, NULL) != NULL)
        return 0;
    hash = HashMapCreate(10, HashFunc, Equality);
    if(NULL == hash){
        return 0;
    }
    if(hash->m_hashSize != 10){
        return 0;
    }
    if(hash->m_numOfItems != 0){
        return 0;
    }
    if(hash->m_magic != MAGIC){
        return 0;
    }
    HashMapDestroy(&hash, Destroyer, Destroyer);
    return 1;
}


/* Check TreeInsert function */
int InsertTest1(){
    HashMap *hash;
    int i, j, **arr1, **arr2;
    hash = HashMapCreate(10, HashFunc, Equality);
    arr1 = IntArray();
    arr2 = IntArray();
    if(HashMapInsert(NULL, arr1[0], arr2[0]) != MAP_UNINITIALIZED_ERROR)
        return 0;
    for(i = 0; i < 10; i++){
        if(HashMapInsert(hash, arr1[i], arr2[i]) != MAP_SUCCESS)
            return 0;
        if(i < 9 && HashMapInsert(hash, arr1[i], arr2[i]) != MAP_KEY_DUPLICATE_ERROR)
            return 0;
    }
    if(hash->m_numOfItems != 10)
        return 0;
    HashMapDestroy(&hash, Destroyer, Destroyer);
    return 1;
}


/* Check TreeIsDataFound() function */
int RemoveTest1(){
    HashMap *hash;
    void *temp1;
    int i, j, *temp, **arr1, **arr2 = IntArray();
    arr1 = IntArray();
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp[0] = j;
    temp1 = &j;
    hash = HashMapCreate(10, HashFunc, Equality);
    if(HashMapRemove(hash, &temp1, arr1[8], arr2[8]) != MAP_KEY_NOT_FOUND_ERROR)
        return 0;
    for(i = 0; i < 10; i++){
        HashMapInsert(hash, arr1[i], arr2[i]);
    }
    if(HashMapRemove(NULL, &temp1, arr1[8], arr2[8]) != MAP_UNINITIALIZED_ERROR)
        return 0;
    if(HashMapRemove(hash, arr1[8], arr1[8], arr2[8]) != MAP_SUCCESS)
        return 0;
    if(HashMapRemove(hash, arr1[8], arr1[8], arr2[8]) != MAP_KEY_NOT_FOUND_ERROR)
        return 0;
    HashMapDestroy(&hash, Destroyer, Destroyer);
    return 1;
}

int HashFoundTest(){
    HashMap *hash;
    int i, j, **arr1, **arr2 = IntArray();
    void *temp, *temp1;
    arr1 = IntArray();
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp = &j;
    temp1 = &j;
    hash = HashMapCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashMapInsert(hash, arr1[i], arr2[i]);
    }
    if(HashMapFind(NULL, arr1[8], temp1) != MAP_UNINITIALIZED_ERROR)
        return 0;
    if(HashMapFind(hash, arr1[8], temp1) != MAP_SUCCESS)
        return 0;
    if(HashMapFind(hash, NULL, temp1) != MAP_KEY_NULL_ERROR)
        return 0;
    if(HashMapRemove(hash, arr1[8], arr1[8], temp1) != MAP_SUCCESS)
        return 0;
    if(HashMapFind(hash, arr1[8], temp1) != MAP_KEY_NOT_FOUND_ERROR)
        return 0;
    HashMapDestroy(&hash, Destroyer, Destroyer);
    return 1;
}

int HashNumOfItemsTest(){
    HashMap *hash;
    int i, j, *temp, **temp1, **arr1, **arr2 = IntArray();
    arr1 = IntArray();
    temp = (int *)malloc(sizeof(int));
    j = 11;
    temp[0] = j;
    hash = HashMapCreate(10, HashFunc, Equality);
    for(i = 0; i < 10; i++){
        HashMapInsert(hash, arr1[i], arr2[i]);
    }
    if(HashMapSize(NULL) != 0)
        return 0;
    if(HashMapSize(hash) != 10)
        return 0;
    HashMapDestroy(&hash, Destroyer, Destroyer);
    return 1;
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
}
