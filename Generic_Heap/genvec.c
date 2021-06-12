# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include "genvec.h"

# define MAGIC_NUMBER 2987640
# define MEMORY_BLOCK 2


struct Vector
{
    void**  m_items;          /* a pointer to allocated array of type int */
    size_t  m_originalSize;   /* original allocated space for items)	*/
    size_t  m_size;           	    /* actual allocated space for items) */
    size_t  m_nItems;     	    /* actual number of items */
    size_t  m_blockSize;	   /* the chunk size to be allocated when no space*/
    size_t m_magicNumber;
};

static void VectorAdjastment(Vector *_vector,  void** _item);

/* size      - The initial allocation size.
   blockSize - The extention block size.    */
Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize){
    struct Vector *ptr;
    if((_initialCapacity == 0) && (_blockSize == 0)){
        return NULL;
    }
    ptr = (struct Vector *)malloc(sizeof(struct Vector));
    if(ptr == NULL){
        return NULL;
    }
    ptr->m_items = (void **)calloc(_initialCapacity, sizeof(void *));
    if(ptr->m_items == NULL){
        return NULL;
    }
    ptr->m_originalSize = _initialCapacity;
    ptr->m_size = _initialCapacity;
    ptr->m_nItems = 0;
    ptr->m_blockSize = _blockSize;
    ptr->m_magicNumber = MAGIC_NUMBER;
    return ptr;
}


void VectorDestroy(Vector** _vector,  void (*_elementDestroy)(void* _item)){
    int i = 0;
    if(_vector != NULL && (*_vector)->m_magicNumber == MAGIC_NUMBER){
        if((*_vector)->m_items != NULL){
            if(_elementDestroy != NULL){
                for(; i < (*_vector)->m_nItems; i++){
                    _elementDestroy((*_vector)->m_items[i]);
                }
                free((*_vector)->m_items);
            }
        }
        (*_vector)->m_magicNumber = 0;
        free(*_vector);
    }
    _vector = NULL;
}

/* Add item to end. */
VectorResult VectorAppend(Vector *_vector,  void *_item){
    void **ptr;
    if(_vector == NULL || _item == NULL){
        return VECTOR_UNINITIALIZED_ERROR;
    }
    if(_vector->m_nItems == _vector->m_size && _vector->m_blockSize > 0){
        ptr = realloc(_vector->m_items, (_vector->m_size + _vector->m_blockSize) * sizeof(void *));
        if(ptr == NULL){
            return VECTOR_ALLOCATION_ERROR;
        } 
        else{
            _vector->m_items = ptr;
            _vector->m_items[_vector->m_nItems] = _item;
            _vector->m_nItems += 1;
            _vector->m_size += _vector->m_blockSize;
        }
    }
    else if(_vector->m_nItems == _vector->m_size && _vector->m_blockSize < 1){
        return VECTOR_OVERFLOW_ERROR;
    }
    else{
        _vector->m_items[_vector->m_nItems] = _item;
        _vector->m_nItems += 1;
    }
    return VECTOR_SUCCESS;
}


/* Delete item from the end. */
VectorResult VectorRemove(Vector *_vector,  void** _item){
    int size = 0, left;
    void **ptr;
    if(_vector == NULL || _item == NULL){
        return VECTOR_UNINITIALIZED_ERROR;
    }
    size = ((_vector->m_size - _vector->m_nItems + 1) / _vector->m_blockSize);
    left = (_vector->m_nItems - 1 - _vector->m_originalSize) % _vector->m_blockSize;

    if(_vector->m_nItems == 0){
        return VECTOR_UNDERFLOW_ERROR;
    }
    else{
        VectorAdjastment(_vector, _item);
        return VECTOR_SUCCESS;   
    }
}

/* The Index range in the following functions is : 0..numOfItems-1 */
VectorResult VectorGet(const Vector *_vector, size_t _index, void** _item){
    if(_vector == NULL || _item == NULL){
        return VECTOR_UNINITIALIZED_ERROR;
    }
    else if(_vector->m_nItems <= _index){
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    *_item = (_vector->m_items[_index]);
    return VECTOR_SUCCESS;
} /* _item = get the value that is in the relevent index */

/* Re-set the value in the given _index with the given value that is _item */
VectorResult VectorSet(struct Vector *_vector, size_t _index, void *_item){
    if(_vector == NULL){
        return VECTOR_UNINITIALIZED_ERROR;
    }
    else if(_vector->m_nItems <= _index){
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    else{
        _vector->m_items[_index] = _item;
        return VECTOR_SUCCESS;
    }
}

/* Return the amount of elements inside the vector */
size_t VectorSize(const Vector *_vector){
    if(_vector != NULL){
        return _vector->m_nItems;
    }
    return 0;
}


size_t VectorCapacity(const Vector* _vector){
    if(_vector != NULL){
        return _vector->m_size;
    }
    return 0;
}


size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context){
    int i;
    if(_vector != NULL && _context != NULL && _vector->m_items != NULL){
        for(i = 0; i < _vector->m_nItems; ++i){
            if(_action(_vector->m_items[i], i, _context) == 0)
                break;
        }
        return i;
    }
    return (_vector->m_size + 1);
}


/*  Unit-Test functions  */
void VectorPrint(Vector *_vector, void (*_elementDestroy)(void* _item)){
    int i;
    if(_vector != NULL){
        for(i = 0; i< _vector->m_nItems; i++){
            _elementDestroy(_vector->m_items[i]);
        }
    }
    putchar('\n');
}

static void VectorAdjastment(Vector *_vector,  void** _item){
    if((_vector->m_nItems - 1) > _vector->m_originalSize && size > 2){
        ptr = realloc(_vector->m_items, (_vector->m_size - size + 2 * _vector->m_blockSize) * sizeof(int));
        if(ptr != NULL){
            _vector->m_items = ptr;
            _vector->m_size = _vector->m_size - size + 2 * _vector->m_blockSize;
        }
        _vector->m_nItems -= 1;
        *_item = (_vector->m_items[_vector->m_nItems]);
    }
    else if((_vector->m_size - _vector->m_originalSize) > 2){
        ptr = realloc(_vector->m_items, (_vector->m_originalSize + 2 * _vector->m_blockSize) * sizeof(int));
        if(ptr != NULL){
            _vector->m_items = ptr;
            _vector->m_size = _vector->m_originalSize + 2 * _vector->m_blockSize;
        }
        _vector->m_nItems -= 1;
        *_item = (_vector->m_items[_vector->m_nItems]);
    }
    else{
        _vector->m_nItems -= 1;
        *_item = (_vector->m_items[_vector->m_nItems]);
    }
}


/* For Test Purposes */
void **GetM_items(Vector *_vector){
    return _vector->m_items;
}

/* For Test Purposes */
size_t GetM_originalSize(Vector *_vector){
    return _vector->m_originalSize;
}

/* For Test Purposes */
size_t GetM_size(Vector *_vector){
    return _vector->m_size;
}

/* For Test Purposes */
size_t GetM_nItems(Vector *_vector){
    return _vector->m_nItems;
}

/* For Test Purposes */
size_t GetM_blockSize(Vector *_vector){
    return _vector->m_blockSize;
}

/* For Test Purposes */
size_t GetM_MagicNumber(Vector *_vector){
    return _vector->m_magicNumber;
}