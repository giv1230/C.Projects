# include <stddef.h>
# include <stdio.h>
# include <stdlib.h> /* for size_t */
# include "genqueue.h"

# define MAGICNUMBER 1594086




struct Queue
{
    void** m_que;      /* A pointer to an Array. */
    size_t m_size;     /* Size of Array */
    size_t m_head;     /* Index of head in m_que. */
    size_t m_tail;     /* Index of tail in m_que. */
    size_t m_nItems;   /* Number Of Items. */
    int m_magicNumber;  /* A number that is used to make sure that there 
                           is no double free of allocated memory */
};


Queue* QueueCreate(size_t _size){
    Queue *qu = NULL;
    qu = (Queue *)malloc(sizeof(Queue));
    if(NULL == qu){
        return NULL;
    }
    if(2 > _size){
        return NULL;
    }
    qu->m_que = (void **)malloc(sizeof(void *) * _size);
    if(NULL == qu->m_que){
        return NULL;
    }
    qu->m_size = _size;
    qu->m_head = 0;
    qu->m_tail = 0;
    qu->m_nItems = 0;
    qu->m_magicNumber = MAGICNUMBER;
    return qu;
}


void QueueDestroy(Queue* _queue, DestroyItem _itemDestroy){ /* TODO: _itemDestroy */
    int i;
    if(NULL != _queue && _queue->m_magicNumber == MAGICNUMBER){
        if(NULL != _queue->m_que){
            for(i = 0; i < _queue->m_nItems; i++){
                _itemDestroy(_queue->m_que[i]);
            }
            free(_queue->m_que);
        }
        _queue->m_magicNumber = 0;
        free(_queue);
    }
}


QueueResult QueueInsert(Queue *_queue, void* _item){
    if(NULL == _queue || NULL == _queue->m_que){
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if(_queue->m_size == _queue->m_nItems){
        return QUEUE_OVERFLOW_ERROR;
    }
    _queue->m_que[_queue->m_tail] = _item;
    _queue->m_tail = (_queue->m_tail + 1) % _queue->m_size;
    _queue->m_nItems++;
    return QUEUE_SUCCESS;
}


QueueResult QueueRemove(Queue *_queue, void** _item){
    if(NULL == _queue || NULL == _queue->m_que){
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if(_queue->m_nItems == 0){
        return QUEUE_UNDERFLOW_ERROR;
    }
    _item = &(_queue->m_que[_queue->m_head]); /*
    _queue->m_que[_queue->m_head] = NULL; */
    _queue->m_nItems--;
    _queue->m_head = (_queue->m_head + 1) % _queue->m_size;
    return QUEUE_SUCCESS;
}
/*  
    b genqueue.c:74
*/

size_t QueueIsEmpty(Queue *_queue){
    if(NULL == _queue || NULL == _queue->m_que){
        return 0;
    }
    return _queue->m_nItems;
}


void QueuePrint(Queue *_queue){
    size_t i = _queue->m_head;
    if(NULL != _queue || NULL != _queue->m_que){
        while(i != _queue->m_nItems){
            printf("%d, ", *(int *)_queue->m_que[i]);
            i = (i + 1) % _queue->m_size;
        }
        putchar('\n');
    }
}

size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context){
    int i, j, count = 0;
    if(NULL != _queue && NULL != _action & NULL != _context){
        i = _queue->m_head;
        j = (_queue->m_tail + 1) % _queue->m_size;
        do{
            count++;
            if(_action(_queue->m_que[i], _context) == 0)
                return count;
            i = (i + 1) % _queue->m_size;
        }while(i != j);
        return (_queue->m_size + 1);
    }
}

void **GetMQueue(Queue *_queue){
    return _queue->m_que;
}

size_t GetSize(Queue *_queue){
    return _queue->m_size;
}

size_t GetHead(Queue *_queue){
    return _queue->m_head;
}

size_t GetTail(Queue *_queue){
    return _queue->m_tail;
}

size_t GetNItems(Queue *_queue){
    return _queue->m_nItems;
}

size_t GetMagicNum(Queue *_queue){
    return _queue->m_magicNumber;
}
