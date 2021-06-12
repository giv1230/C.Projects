# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "list.h"
# include "list_internal.h"


# define MAGICNUM 721985



List* ListCreate(void){
    List *list = NULL;
    list = (List *)malloc(sizeof(List));
    if (NULL == list){
        return NULL;
    }
    list->magicNumber = MAGICNUM;
    list->m_head.m_prev = &list->m_head;
    list->m_head.m_next = &list->m_tail;
    list->m_head.m_data = NULL;
    list->m_head.magicNumber = MAGICNUM;
    list->m_tail.m_next = &list->m_tail;
    list->m_tail.m_prev = &list->m_head;
    list->m_tail.m_data = NULL;
    list->m_tail.magicNumber = MAGICNUM;
    return list;
}

/* I've added a pointer to function */
void ListDestroy(List** _list, void (*_elementDestroy)(void* _item)){ 
    void *data;
    int i = 0;
    data = &i;
    if (NULL != _list){
        if((*_list)->magicNumber == MAGICNUM){
            while(ListPopHead((*_list), data) != LIST_UNDERFLOW_ERROR){
                _elementDestroy(data);
            }
            (*_list)->magicNumber = 0;
            free(*_list);
        }
    }
}

ListResult ListPushHead(List* _list, void* _item){
    Node *n = NULL;
    if(NULL == _list){
        return LIST_UNINITIALIZED_ERROR;
    }
    if(NULL == _item){
        return LIST_NULL_ELEMENT_ERROR;
    }
    n = (Node *)malloc(sizeof(Node));
    if(NULL == n){
        return LIST_ALLOCATION_ERROR;
    }
    n->m_data = _item;
    n->magicNumber = MAGICNUM;
    n->m_prev = &_list->m_head;
    n->m_next = _list->m_head.m_next;
    _list->m_head.m_next->m_prev = n;
    _list->m_head.m_next = n;
    return LIST_SUCCESS;
}


ListResult ListPushTail(List* _list, void* _item){
    Node *n = NULL;
    if(NULL == _list){
        return LIST_UNINITIALIZED_ERROR;
    }
    if(NULL == _item){
        return LIST_NULL_ELEMENT_ERROR;
    }
    n = (Node *)malloc(sizeof(Node));
    if(NULL == n){
        return LIST_ALLOCATION_ERROR;
    }
    n->m_data = _item;
    n->magicNumber = MAGICNUM;
    n->m_next = &_list->m_tail;
    n->m_prev = _list->m_tail.m_prev;
    _list->m_tail.m_prev->m_next = n;
    _list->m_tail.m_prev = n;
    return LIST_SUCCESS;
}


ListResult ListPopHead(List* _list, void** _data){
    Node *ptr = NULL;
    if(NULL == _list){
        return LIST_UNINITIALIZED_ERROR;
    }
    if(NULL == _data){
        return LIST_NULL_ELEMENT_ERROR;
    }
    if(NULL != _list->m_head.m_next->m_data && MAGICNUM == _list->m_head.m_next->magicNumber){ 
        ptr = _list->m_head.m_next;
        ptr->magicNumber = 0;
        /* a way to identify if the head point to the tail or not */
        *_data = _list->m_head.m_next->m_data;
        /* The second item in the list get pointer (m_prev) to the Head */
        _list->m_head.m_next->m_next->m_prev = &_list->m_head; 
        /* The head get point (m_next) to the second item in the list */
        _list->m_head.m_next = _list->m_head.m_next->m_next;
        free(ptr);
        return LIST_SUCCESS;
    }
    return LIST_UNDERFLOW_ERROR;
}


ListResult ListPopTail(List* _list, void** _data){
    Node *ptr = NULL;
    if(NULL == _list){
        return LIST_UNINITIALIZED_ERROR;
    }
    if(NULL == _data){
        return LIST_NULL_ELEMENT_ERROR;
    }
    if(NULL != _list->m_tail.m_prev->m_data && MAGICNUM == _list->m_tail.m_prev->magicNumber){ 
        ptr = _list->m_tail.m_prev;
        ptr->magicNumber = 0;
        /* a way to identify if the head point to the tail or not */
        _data = &_list->m_tail.m_prev->m_data;
        /* The second-last item get pointer (m_next) to the tail */
        _list->m_tail.m_prev->m_prev->m_next = &_list->m_tail; 
        /* The tail get point (m_prev) to the second-last item in the list */
        _list->m_tail.m_prev = _list->m_tail.m_prev->m_prev; 
        free(ptr);
        return LIST_SUCCESS;
    }
    return LIST_UNDERFLOW_ERROR;
}


size_t ListSize(const List* _list){
    Node* n;
    size_t count = 0;
    if (NULL == _list){
        return count;
    }
    n = _list->m_head.m_next;
    while (NULL != n->m_data)
    {
        count++;
        n = n->m_next;
    }
    return count;
}


int ListIsEmpty(const List* _list){
    if (NULL == _list){
        return 1;
    }
    if (0 == ListSize(_list))
    {
        return 1;
    }
    return 0;
}

/* ListPrint is only for debug */
void ListPrint(List* _list){
    int i = 0;
    Node* n = NULL;
    if(NULL != _list){
        n = _list->m_head.m_next;
        while (NULL != n->m_data)
        {
            i = *(int *)n->m_data;
            printf("%d, ", i);
            n = n->m_next;
        }
    }
    
}
