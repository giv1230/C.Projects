# include <stddef.h>
# include <stdlib.h>
# include <math.h>
# include "hashset.h"

# define MAGIC 248968

# define MARGIN 1.3

struct Hash
{
    void** m_data;
    char* m_state;
    size_t (*m_hashFunction)(void* _data);
    int (*m_equalityFunction)(void* _firstData, void* _secondData);
    size_t m_hashSize; /*original size given by the client*/
    size_t m_capacity; /*real hash size */
    size_t m_numOfItems; /*number of occupied places in the table*/
    size_t m_maxOfRehashOperations; /*maximum amount of rehash operations which have been done over one insertion*/
    size_t m_counterOfInsertions; /*total amount of insertions into the table*/
    size_t m_allRehashOperations; /*all rehash operations mutual counter*/
    int m_magic; /*magic Number for prevention of double free of allocated memory */
};

static int IsDuplicate(Hash* _hash, void* _data);
static int Iteration(const Hash* _hash, void* _key);



Hash* HashCreate(size_t _size, size_t (*_hashFunction)(void*), int (*_equalityFunction)(void*, void*)){
    Hash* hash;
    int cap = 0;
    if(NULL == _hashFunction || NULL == _equalityFunction || 2 > _size)
        return NULL;
    hash = (Hash *)malloc(sizeof(Hash));
    if(NULL == hash)
        return NULL;
    cap = _size * MARGIN;
    hash->m_capacity = Capacity(cap);
    hash->m_data = (void **)malloc(sizeof(void *) * hash->m_capacity);
    if(NULL == hash->m_data){
        free(hash);
        return NULL;
    }
    hash->m_state = calloc(hash->m_capacity, sizeof(void *));
    if(NULL == hash->m_state){
        free(hash->m_data);
        free(hash);
        return NULL;
    }
    hash->m_hashFunction = _hashFunction;
    hash->m_equalityFunction = _equalityFunction;
    hash->m_hashSize = _size;
    hash->m_numOfItems = 0;
    hash->m_maxOfRehashOperations = 0;
    hash->m_counterOfInsertions = 0;
    hash->m_allRehashOperations = 0;
    hash->m_magic = MAGIC;
    return hash;
}


void HashDestroy(Hash** _hash, void (*_keyDestroy)(void* _data)){
    int i;
    if(NULL != _hash && (*_hash)->m_magic == MAGIC){
        if(NULL != _keyDestroy && (*_hash)->m_numOfItems > 0 && NULL != (*_hash)->m_data){
            for(i = 0; i < (*_hash)->m_capacity; i++){
                if((*_hash)->m_data[i] != NULL)
                    _keyDestroy((*_hash)->m_data[i]);
            }
        }
        (*_hash)->m_magic = 0;
        if(NULL != (*_hash)->m_data)
            free((*_hash)->m_data);
        if(NULL != (*_hash)->m_state)
            free((*_hash)->m_state);
        free(*_hash);
    }
}


HashSet_Result HashInsert(Hash* _hash, void* _data){
    size_t index, rehash = 0;
    if(NULL == _hash || NULL == _data){
        return SET_UNINITIALIZED;
    }
    if(_hash->m_numOfItems == _hash->m_hashSize)
        return SET_OVERFLOW;
    index = _hash->m_hashFunction(_data) % _hash->m_capacity;
    if(Iteration(_hash, _data) >= 0)
        return SET_KEY_DUPLICATE;
    while(_hash->m_state[index] == '1'){
        index = (index + 1) % _hash->m_capacity;
        _hash->m_allRehashOperations += 1;
        rehash++;
    }
    _hash->m_data[index] = _data;
    _hash->m_state[index] = '1';
    _hash->m_numOfItems += 1;
    _hash->m_counterOfInsertions += 1;

    if(rehash > _hash->m_maxOfRehashOperations)
        _hash->m_maxOfRehashOperations = rehash;
    return SET_SUCCESS;
}


HashSet_Result HashRemove(Hash* _hash, void* _key, void** _data){
    int index;
    if(NULL == _hash || NULL == _key || NULL == _data)
        return SET_UNINITIALIZED;
    if(_hash->m_numOfItems == 0)
        return SET_UNDERFLOW;
    index = Iteration(_hash, _key);
    if(index >= 0){
        _data = _hash->m_data[index];
        _hash->m_data[index] = NULL;
        _hash->m_state[index] = '2';
        _hash->m_numOfItems -= 1;
        return SET_SUCCESS;
    }
    return SET_KEY_NOT_FOUND;
}


int HashIsFound(const Hash* _hash, void* _key){
    if(NULL == _hash || NULL == _key || _hash->m_numOfItems == 0){
        return 0;
    }
    if(Iteration(_hash, _key) >= 0)
        return 1;
    return 0;
}


size_t HashNumOfItems(const Hash* _hash){
    if(NULL != _hash)
        return _hash->m_numOfItems;
    return 0;
}


size_t HashCapacity(const Hash* _hash){
    if(NULL != _hash)
        return _hash->m_hashSize;
    return 0;
}


double HashAverageRehashes(const Hash* _hash){
    if(NULL != _hash && _hash->m_counterOfInsertions != 0)
        return (_hash->m_allRehashOperations / _hash->m_counterOfInsertions);
    return 0;
}


size_t HashMaxReHash(const Hash* _hash){
    if(NULL != _hash)
        return _hash->m_maxOfRehashOperations;
    return 0;
}



void HashPrint(const Hash* _hash, void (*PrintKey)(void*)){
    size_t index = 0;
    if(NULL != _hash && NULL != PrintKey){
        while(index < _hash->m_capacity){
            if(_hash->m_state[index] == 1)
                PrintKey(_hash->m_data[index]);
            index++;
        }
    }
}

/*  This function calculate the actual size of the hash table (it's different than 
 *  the user's requested size).
 *  It takes the closet Prime number to _size * MARGIN.
 */
int Capacity(const int _size){
    int i = 5, s = (int)sqrt((double)_size);
    if(_size % s == 0 || _size % 3 == 0 || _size % 2 == 0){
        return Capacity(_size + 1);
    }
    while(s > i){
        if(_size % i == 0)
            return Capacity(_size + 1);
        i + 2;
    }
    return _size;
}

/* Returns index of key if it found, otherwise the function returns -1 */
static int Iteration(const Hash* _hash, void* _key){
    int index;
    if(NULL == _hash || NULL == _key || _hash->m_numOfItems == 0){
        return -1;
    }
    index = _hash->m_hashFunction(_key) % _hash->m_capacity;
    while(_hash->m_state[index] == '1' || _hash->m_state[index] == '2'){
        if(_hash->m_state[index] == '1'  && _hash->m_equalityFunction(_hash->m_data[index], _key) == 1)
            return index;
        index = (index + 1) % _hash->m_capacity;
    }
    return -1;
}