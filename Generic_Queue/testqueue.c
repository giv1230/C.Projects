# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include "genqueue.h"


void **GetMQueue(Queue *_queue);
size_t GetSize(Queue *_queue);
size_t GetHead(Queue *_queue);
size_t GetTail(Queue *_queue);
size_t GetNItems(Queue *_queue);
size_t GetMagicNum(Queue *_queue);


int Compare(const void *_left, const void *_right){
    if(*(int *)_left >= *(int *)_right){
        return 1;
    }
    else{
        return 0;
    }
}

int QueueFind(const void* _element,void *_context){
    if(_element == NULL || _context == NULL){
        return -1;
    }
    if(*(int *)_element == *(int *)_context){
        return 0;
    }
    return 1;
}

void Destroyer(void* _element){
    _element;
}



/* Check if it works */
int CreateTest1(){
    Queue *new = NULL;
    /* check Size = 1 */
    new = QueueCreate(1);
    if(new != NULL){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    /* Check if it works */
    new = QueueCreate(10);
    if(new == NULL){
        return 0;
    }
    if(QueueIsEmpty(new) != 0){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    if(GetMQueue(new) == NULL || GetSize(new) != 10 || GetHead(new) != 0 || GetTail(new) != 0 || GetNItems(new) !=0){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    QueueDestroy(new, Destroyer);
    return 1;
}

/* Check Insert Works */
int InsertTest1(){
    size_t i;
    Queue *new = NULL;
    new = QueueCreate(20);
    /* Check Null Pointer */
    if(QueueInsert(NULL, &i) != QUEUE_UNINITIALIZED_ERROR){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    for(i = 1; i < 25; i++){
        /* Check Insert Works */
        if(i <= 20 && QueueInsert(new, &i) != QUEUE_SUCCESS){
            QueueDestroy(new, Destroyer);
            return 0;
        }
        /* Check if Over Flow work */
        if(i > 20 && QueueInsert(new, &i) != QUEUE_OVERFLOW_ERROR){
            QueueDestroy(new, Destroyer);
            return 0;
        }
    }
    if(GetNItems(new) != 20){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    QueueDestroy(new, Destroyer);
    return 1;
}


/* Checks if Insert work on circle */
int InsertTest2(){
    void *item;
    size_t i, j = 0;
    Queue *new = NULL;
    new = QueueCreate(25);
    for(i = 1; i < 25; i++){
        QueueInsert(new, &i);
    }
    for(j = 1; j < 7; j++){
        item = &i;
        QueueRemove(new, item);
    }
    for(i = 1; i < 5; i++){
        if(QueueInsert(new, &i) != QUEUE_SUCCESS){
            QueueDestroy(new, Destroyer);
            return 0;
        }
    }
    QueueDestroy(new, Destroyer);
    return 1;
}

/* Check NULL pointer */
int RemoveTest1(){
    void *temp;
    Queue *new = NULL;
    /* Check NULL pointer */
    if(QueueRemove(NULL, &temp) != QUEUE_UNINITIALIZED_ERROR){
        return 0;
    }
    new = QueueCreate(25);
    /* Checks Under Flow */
    if(QueueRemove(new, &temp) != QUEUE_UNDERFLOW_ERROR){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    QueueDestroy(new, Destroyer);
    return 1;
}

/* Check Null Pointer */
int EmptyTest1(){
    Queue *new = NULL;
    new = QueueCreate(25);
    /* Check Null Pointer */
    if(QueueIsEmpty(NULL) != 0){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    /* Check Correctness */
    if(QueueIsEmpty(new) != 0){
        QueueDestroy(new, Destroyer);
        return 0;
    }
    return 1;
}

int ForEachTest(){
    Queue *new = NULL;
    int item1 = 1, item2 = 6, item3 = 9, item4 = 45, item5 = 28, item6 = 12, item7 = 13, item8 = 85, test = 12;
    new = QueueCreate(15);
    QueueInsert(new, &item1);
    QueueInsert(new, &item2);
    QueueInsert(new, &item3);
    QueueInsert(new, &item4);
    QueueInsert(new, &item5);
    QueueInsert(new, &item6);
    QueueInsert(new, &item7);
    QueueInsert(new, &item8);
    if(QueueForEach(new, QueueFind, &test) >= 8){
        QueueDestroy(new, Destroyer);
        return 0;
    }
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

    /* IsertTest */
    i = InsertTest1();
    if(i == 1){
        printf("InsertTest1 = OK\n");
    }
    else{
        printf("InsertTest1 = FAIL\n");
    }

    i = InsertTest2();
    if(i == 1){
        printf("InsertTest2 = OK\n");
    }
    else{
        printf("InsertTest2 = FAIL\n");
    }
    
    /* RemoveTest */
    i = RemoveTest1();
    if(i == 1){
        printf("RemoveTest1 = OK\n");
    }
    else{
        printf("RemoveTest1 = FAIL\n");
    }

    /* EmptyTest */
    i = EmptyTest1();
    if(i == 1){
        printf("EmptyTest1 = OK\n");
    }
    else{
        printf("EmptyTest1 = FAIL\n");
    }

    i = ForEachTest();
    if(i == 1){
        printf("ForEachTest = OK\n");
    }
    else{
        printf("ForEachTest = FAIL\n");
    }
}


