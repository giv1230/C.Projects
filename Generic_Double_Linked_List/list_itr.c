# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "list.h"
# include "list_itr.h"
# include "list_internal.h"


ListItr ListItrBegin(const List* _list){
    if(NULL == _list){
        return NULL;
    }
    if(&_list->m_tail == _list->m_head.m_next){
        return &_list->m_tail;
    }
    return _list->m_head.m_next;
}

ListItr ListItrEnd(const List* _list){
    if(NULL == _list){
        return NULL;
    }
    return &_list->m_tail;
}

int ListItrEquals(const ListItr _a, const ListItr _b){
    if(_a == _b)
        return 1;
    return 0;
}

ListItr ListItrNext(ListItr _itr){
    Node *n;
    if(NULL == _itr)
        return NULL;
    n = (Node *)_itr;
    if(n->m_data == NULL && n->m_next == NULL)
        return &n;
    return n->m_next;
}

ListItr ListItrPrev(ListItr _itr){
    Node *n;
    if(NULL == _itr)
        return NULL;
    n = (Node *)_itr;
    if(n->m_data == NULL && n->m_prev == NULL)
        return &n;
    return n->m_prev;
}

void* ListItrGet(ListItr _itr){
    Node *n;
    if(NULL == _itr)
        return NULL;
    n = (Node *)_itr;
    return n->m_data;
}

void* ListItrSet(ListItr _itr, void* _element){
    Node *n;
    void *data;
    if(NULL == _itr || NULL == _element)
        return NULL;
    n = (Node *)_itr;
    data = n->m_data;
    n->m_data = _element;
    return data;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element){
    Node *temp1, *temp2;
    if(NULL == _itr || NULL == _element)
        return NULL;
    temp1 = (Node *)_itr;
    temp2 = (Node *)_element;
    temp2->m_next = _itr; /* temp2->next */
    temp2->m_prev = temp1->m_prev; /* temp2->prev */
    temp1->m_prev->m_next = _element; /* x->next */
    temp1->m_prev = _element; /* temp1->prev */
    return _element;
}

void* ListItrRemove(ListItr _itr){
    Node *temp;
    if(NULL == _itr)
        return NULL;
    temp = (Node*)_itr;
    temp->m_next->m_prev = temp->m_prev;
    temp->m_prev->m_next = temp->m_next;
    free(_itr);
    return temp->m_data;
}