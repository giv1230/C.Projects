# include <stdio.h>
# include <stdlib.h>
# include "genvec.h"

/*
    The Test should be based on the requirements of the code or the "h" file
    Also recommended to Test other scenarios
*/
/* For Test Purposes */
void **GetM_items(Vector *_vector);
/* For Test Purposes */
size_t GetM_originalSize(Vector *_vector);
/* For Test Purposes */
size_t GetM_size(Vector *_vector);
/* For Test Purposes */
size_t GetM_nItems(Vector *_vector);
/* For Test Purposes */
size_t GetM_blockSize(Vector *_vector);
/* For Test Purposes */
size_t GetM_MagicNumber(Vector *_vector);


int VectorFind(void* _element, size_t _index, void *_context){
    if(_element != NULL || _context != NULL){
        return -1;
    }
    if(*(int *)_element == *(int *)_context){
        return 0;
    }
    return 1;
}

void VecPrint(void *_item){
    if(_item != NULL ){
        printf("%d, ", *(int *)_item);
    }
}


void FreeArray(void *_arr){
    free(_arr);
}

/* Test Class */

/*** VectorCreate ***/

/* Checks VectorCreate works */
int CreateTest1(){
    Vector *new = NULL;
    /* Check Size = 0 && Block = 0 */
    new = VectorCreate(0, 0);
    if(new != NULL){
        return 0;
    }
    /* Checks VectorCreate works */
    new = VectorCreate(10, 10);
    if(new == NULL){
        return 0;
    }
    if(GetM_originalSize(new) != 10){
        return 0;
    }
    if(GetM_size(new) != 10){
        return 0;
    }
    if(GetM_blockSize(new) != 10){
        return 0;
    }
    if(GetM_nItems(new) != 0){
        return 0;
    }
    printf("m_originalSize = %ld\n", GetM_originalSize(new));
    printf("m_size = %ld\n", GetM_size(new));
    printf("m_nItems = %ld\n", GetM_nItems(new));
    printf("m_blockSize = %ld\n", GetM_blockSize(new));
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}

/* TODO: Check Size = 0 && Block != 0 */
int CreateTest2(){
    Vector *new = NULL;
    new = VectorCreate(10, 0);
    if(new == NULL){
        return 0;
    }
    if(GetM_originalSize(new) != 10){
        return 0;
    }
    if(GetM_size(new) != 10){
        return 0;
    }
    if(GetM_blockSize(new) != 0){
        return 0;
    }
    if(GetM_nItems(new) != 0){
        return 0;
    }
    printf("m_originalSize = %ld\n", GetM_originalSize(new));
    printf("m_size = %ld\n", GetM_size(new));
    printf("m_nItems = %ld\n", GetM_nItems(new));
    printf("m_blockSize = %ld\n", GetM_blockSize(new));
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}

/* TODO: Check Size != 0 && Block = 0 */
int CreateTest3(){
    Vector *new = NULL;
    new = VectorCreate(0, 10);
    if(new == NULL){
        return 0;
    }
    if(GetM_originalSize(new) != 0){
        return 0;
    }
    if(GetM_size(new) != 0){
        return 0;
    }
    if(GetM_blockSize(new) != 10){
        return 0;
    }
    if(GetM_nItems(new) != 0){
        return 0;
    }
    printf("m_originalSize = %ld\n", GetM_originalSize(new));
    printf("m_size = %ld\n", GetM_size(new));
    printf("m_nItems = %ld\n", GetM_nItems(new));
    printf("m_blockSize = %ld\n", GetM_blockSize(new));
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}
/*** VectorCreate ***/


/*** VectorAppend ***/
/* Check if the parameters are updated */
/* Check if the returned pointers are NULL */
int AddTest1(){
    int i;
    void *temp;
    Vector *new = NULL;
    new = VectorCreate(10, 10);
    for(i = 1; i <25; i++){
        temp = &i;
        VectorAppend(new, &i);
        /* Check if the integer is in the right location in the array */
        if(GetM_nItems(new) != i){
            return 0;
        }
        /* Check if the parameters are updated */
        /* Check if the returned pointers are NULL */
        VectorGet(new, i-1, &temp);
        if(i <= 10){
            if(GetM_originalSize(new) != 10){
                printf("GetM_originalSize");
                return 0;
            }
            if(GetM_size(new) != 10){
                printf("GetM_size");
                return 0;
            }
            if(GetM_blockSize(new) != 10){
                printf("GetM_blockSize");
                return 0;
            }
            if(GetM_nItems(new) != i){
                printf("GetM_nItems");
                return 0;
            }

        }
        if(10 < i && 20 >= i){
            if(GetM_originalSize(new) != 10){
                printf("GetM_originalSize");
                return 0;
            }
            if(GetM_size(new) != 20){
                printf("GetM_size");
                return 0;
            }
            if(GetM_blockSize(new) != 10){
                printf("GetM_blockSize");
                return 0;
            }
            if(GetM_nItems(new) != i){
                printf("GetM_nItems");
                return 0;
            }

        }
        if(20 < i && 30 >= i){
            if(GetM_originalSize(new) != 10){
                printf("GetM_originalSize");
                return 0;
            }
            if(GetM_size(new) != 30){
                printf("GetM_size");
                return 0;
            }
            if(GetM_blockSize(new) != 10){
                printf("GetM_blockSize");
                return 0;
            }
            if(GetM_nItems(new) != i){
                printf("GetM_nItems");
                return 0;
            }

        }
    }
    printf("m_originalSize = %ld (Should be 10)\n", GetM_originalSize(new));
    printf("m_size = %ld (Should be 30)\n", GetM_size(new));
    printf("m_nItems = %ld (Should be 24)\n", GetM_nItems(new));
    printf("m_blockSize = %ld (Should be 10)\n", GetM_blockSize(new));
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}

/* TODO: Check if reallocation succeeded by checking if the pointer address is changed -1 */
int AddTest2(){
    int i;
    void **temp, **temp1;
    Vector *new = NULL;
    new = VectorCreate(5, 5);
    temp = GetM_items(new);
    for(i = 1; i < 7; i++){
        VectorAppend(new, &i);
    }
    temp1 = GetM_items(new);
    if(&temp == &temp1){
        return 0;
    }
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}
/*** VectorAppend ***/



/*** VectorRemove ***/

/* Check if the returned pointers are NULL */
int DeleteTest1(){
    int i;
    void *temp, **tr;
    Vector *new = NULL;
    new = VectorCreate(10, 10);
    tr = GetM_items(new);
    /* Check underFlow when the m_nItems = 0 */
    if(VECTOR_UNDERFLOW_ERROR != VectorRemove(new, &temp)){
        return 0;
    }
    if(new == NULL || tr == NULL){
        return 0;
    }
    for(i = 1; i <25; i++){
        VectorAppend(new, &i);
    }
    /* Check if the function gets NULL pointers */
    if(VectorRemove(NULL, &temp) != VECTOR_UNINITIALIZED_ERROR){
        return 0;
    }
    if(VectorRemove(new, &temp) != VECTOR_SUCCESS){
        return 0;
    }
    /* VectorDestroy(&new, FreeArray); */
    return 1;
}

/* Check if the m_nItems is updated */
int DeleteTest2(){
    int i, temp1, temp2;
    void *temp;
    Vector *new = NULL;
    new = VectorCreate(10, 10);
    for(i = 1; i <25; i++){
        VectorAppend(new, &i);
    }
    temp1 = GetM_nItems(new);
    VectorRemove(new, &temp);
    temp2 = GetM_nItems(new);
    if(temp1 <= temp2){
        return 0;
    }
    if(GetM_nItems(new) != 23){
        return 0;
    }
    return 1;
}

/* Check reallocation with -1 in the m_blockSize */
int DeleteTest3(){
    int i;
    void *temp;
    Vector *new = NULL;
    new = VectorCreate(1, 2);
    for(i = 0; i < 6; i++){
        VectorAppend(new, &i);
    }
    for(i = 0; i < 6; i++){
        if(VECTOR_SUCCESS != VectorRemove(new, &temp)){
            return 0;
        }
    }
    return 1;
}

/*** VectorRemove ***/



/*** VectorGet ***/

/* TODO: Check if the function gets NULL pointers */
int GetTester1(){
    int i, index = 5;
    void *item = NULL;
    Vector *new = NULL;
    new = VectorCreate(10, 10);
    for(i = 1; i < 15; i++){
        VectorAppend(new, &i);
    }
    item = &i;
    if(VectorGet(new, index, &item) != VECTOR_SUCCESS || *(int *) item != 15){
        return 0;
    }
    if(VectorGet(new, 20, &item) != VECTOR_INDEX_OUT_OF_BOUNDS_ERROR){
        return 0;
    }
    return 1;
}

/*** VectorGet ***/



/*** VectorSet ***/

/* TODO: Check if the function gets NULL pointers */
int SetTester1(){
    int i, item = 99, index = 5;
    void *item1;
    Vector *new = VectorCreate(10, 10);
    if(VectorSet(NULL, index, &item) != VECTOR_UNINITIALIZED_ERROR){
        return 0;
    }
    for(i = 1; i < 15; i++){
        VectorAppend(new, &i);
    }
    item1 = &i;
    VectorSet(new, index, &item);
    VectorGet(new, index, &item1);
    /* Check if the parameters are updated */
    if(item != *(int *)item1){
        return 0;
    }
    /* Check with wrong index */
    if(VectorSet(new, 19, &item) != VECTOR_INDEX_OUT_OF_BOUNDS_ERROR){
        return 0;
    }
    return 1;
}

/*** VectorSet ***/



/*** VectorPrint ***/
/* TODO: Check if the function works */
void PrintTest(){
    int i;
    void *temp;
    Vector *new = NULL;
    new = VectorCreate(10, 10);
    for(i = 1; i < 15; i++){
        VectorAppend(new, &i);
    }
    VectorPrint(new, VecPrint);
}
/*** VectorPrint ***/


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
    i = CreateTest2();
    if(i == 1){
        printf("CreateTest2 = OK\n");
    }
    else{
        printf("CreateTest2 = FAIL\n");
    }
    i = CreateTest3();
    if(i == 1){
        printf("CreateTest3 = OK\n");
    }
    else{
        printf("CreateTest3 = FAIL\n");
    }

    /* AddTest */
    i = AddTest1();
    if(i == 1){
        printf("AddTest1 = OK\n");
    }
    else{
        printf("AddTest1 = FAIL\n");
    }
    i = AddTest2();
    if(i == 1){
        printf("AddTest2 = OK\n");
    }
    else{
        printf("AddTest2 = FAIL\n");
    }

    /* DeleteTest */
    i = DeleteTest1();
    if(i == 1){
        printf("DeleteTest1 = OK\n");
    }
    else{
        printf("DeleteTest1 = FAIL\n");
    }
    i = DeleteTest2();
    if(i == 1){
        printf("DeleteTest2 = OK\n");
    }
    else{
        printf("DeleteTest2 = FAIL\n");
    }
    i = DeleteTest3();
    if(i == 1){
        printf("DeleteTest3 = OK\n");
    }
    else{
        printf("DeleteTest3 = FAIL\n");
    }
    
    /* GetTester */
    i = GetTester1();
    if(i == 1){
        printf("GetTester1 = OK\n");
    }
    else{
        printf("GetTester1 = FAIL\n");
    }

    /* SetTester */
    i = SetTester1();
    if(i == 1){
        printf("SetTester1 = OK\n");
    }
    else{
        printf("SetTester1 = FAIL\n");
    }

    PrintTest();
}


