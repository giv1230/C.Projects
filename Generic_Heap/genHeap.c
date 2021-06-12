# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include "genvec.h"
# include "genHeap.h"


# define MAGIC 2880326



struct Heap{
    Vector* m_vec;
    size_t m_heapSize;
    LessThanComparator m_pfLess;
    int m_magic;
};


static void Heapify(Vector* _vec, int _index, LessThanComparator _pfLess);
static void HeapSorter(Vector* _vec, int _index, LessThanComparator _pfLess);
static void Bubbling(Heap *_heap, size_t _index);


/* Receives Vector with values (keys) in it */
Heap* HeapBuild(Vector* _vec, LessThanComparator _pfLess){
    Heap *heap;
    if(_vec == NULL || _pfLess == NULL){
        return NULL;
    }
    heap = (Heap *)malloc(sizeof(Heap));
    if(heap == NULL){
        return NULL;
    }
    heap->m_vec = _vec;
    heap->m_magic = MAGIC;
    heap->m_pfLess = _pfLess;
    heap->m_heapSize = VectorSize(_vec);
    HeapSort(heap->m_vec, _pfLess);
    return heap;
}



Vector* HeapDestroy(Heap** _heap){
    if(_heap != NULL && (*_heap)->m_magic == MAGIC){
        (*_heap)->m_magic = 0;
        free(*_heap);
    }
}


HeapResultCode HeapInsert(Heap* _heap, void* _element){
    VectorResult result;
    if(_heap == NULL || _heap->m_vec == NULL){
        return HEAP_NOT_INITIALIZED;
    }
    result = VectorAppend(_heap->m_vec, _element);
    if(result == VECTOR_ALLOCATION_ERROR){
        return HEAP_REALLOCATION_FAILED;
    }
    Bubbling(_heap, _heap->m_heapSize);
    _heap->m_heapSize += 1;
    return HEAP_SUCCESS;
}


const void* HeapPeek(const Heap* _heap){
    void* data;
    if(_heap != NULL){
        VectorGet(_heap->m_vec, 0, &data);
        return data;
    }
    return NULL;
}


void* HeapExtract(Heap* _heap){
    void* data, *temp;
    if(_heap != NULL || _heap->m_vec != NULL){
        VectorGet(_heap->m_vec, 0, &data);
        VectorRemove(_heap->m_vec, &temp);
        VectorSet(_heap->m_vec, 0, temp);
        _heap->m_heapSize -= 1;
        Heapify(_heap->m_vec, 0, _heap->m_pfLess);
        return data;
    }
    return NULL;
}


size_t HeapSize(const Heap* _heap){
    if(_heap == NULL){
        return 0;
    }
    return _heap->m_heapSize;
}


size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context){
    int i;
    void *temp;
    if(_heap != NULL &&_heap->m_vec != NULL){
        for(i = 0; i < VectorSize(_heap->m_vec); ++i){
            VectorGet(_heap->m_vec, i, &temp);
            if(_act(temp, _context) == 0)
                break;
        }
        return i;
    }
    return 0;
}


void HeapPrint(Heap* _heap){
    if(_heap != NULL || _heap->m_vec != NULL){
        VectorPrint(_heap->m_vec);
    }
}

static void Heapify(Vector* _vec, int _index, LessThanComparator _pfLess){
    void *left = 0, *right = 0, *father = 0;
    if(_vec != NULL || _pfLess != NULL){
        VectorGet(_vec, _index, &father);
        if(VectorSize(_vec) > (_index * 2 + 2)){
            VectorGet(_vec, _index * 2 + 1, &left);
            VectorGet(_vec, _index * 2 + 2, &right);

            if(_pfLess(left, right) && _pfLess(left, father)){
                VectorSet(_vec, _index, left);
                VectorSet(_vec, _index * 2 + 1, father);
                Heapify(_vec, _index * 2 + 1, _pfLess);
            }
            else if(_pfLess(right, left) && _pfLess(right, father)){
                VectorSet(_vec, _index, right);
                VectorSet(_vec, _index * 2 + 2, father);
                Heapify(_vec, _index * 2 + 2, _pfLess);
            }
        }
        else if(VectorSize(_vec) > (_index * 2 + 1)){
            VectorGet(_vec, _index * 2 + 1, &left);
            if(_pfLess(left, father)){
                VectorSet(_vec, _index, left);
                VectorSet(_vec, _index * 2 + 1, father);
                Heapify(_vec, _index * 2 + 1, _pfLess);
            }
        }
    }
}

static void Bubbling(Heap *_heap, size_t _index){
    void *temp, *father;
    size_t i;
    if(_heap != NULL && _heap->m_vec != NULL){
        VectorGet(_heap->m_vec, _index, &temp);
        if((_index % 2) == 0){
            i = ((_index - 2) / 2);
        }
        else{
            i = ((_index - 1) / 2);
        }
        VectorGet(_heap->m_vec, i, &father);
        if(_heap->m_pfLess(temp, father) == 1){ 
            VectorSet(_heap->m_vec, i, temp);
            VectorSet(_heap->m_vec, _index, father);
            if(i > 0){
                Bubbling(_heap, i);
            }
        }
    }
}

void HeapSort(Vector* _vec, LessThanComparator _pfLess){
    if(_vec != NULL || _pfLess != NULL){
        HeapSorter(_vec, 0, _pfLess);
    }
}

static void HeapSorter(Vector* _vec, int _index, LessThanComparator _pfLess){
    if(_vec != NULL || _pfLess != NULL){
        if(VectorSize(_vec) > (_index * 2 + 2)){
            HeapSorter(_vec, (_index * 2 + 1), _pfLess);
            HeapSorter(_vec, (_index * 2 + 2), _pfLess);
            Heapify(_vec, _index, _pfLess);
        }
        else if(VectorSize(_vec) > (_index * 2 + 1)){
            HeapSorter(_vec, (_index * 2 + 1), _pfLess);
            Heapify(_vec, _index, _pfLess);
        }
        else{
            Heapify(_vec, _index, _pfLess);
        }
    }
}