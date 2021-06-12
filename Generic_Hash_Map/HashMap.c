# include <stddef.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include "HashMap.h"
# include "list.h"
# include "list_itr.h"

# define MAGIC 248968



static int Capacity(const int _size);
static Data* Iteration(const HashMap* _hash, void* _key);
static void DestroyDataInList(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value));

HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc){
    HashMap *hash;
    int size;
    if(NULL == _hashFunc || NULL == _keysEqualFunc || _capacity < 2)
        return NULL;
    hash = (HashMap*)malloc(sizeof(HashMap));
    if(NULL == hash)
        return NULL;
    size = Capacity(_capacity);
    hash->m_data = (List **)calloc(size, sizeof(List*));
    if(NULL == hash){
        free(hash);
        return NULL;
    }
    hash->m_hashFunction = _hashFunc;
    hash->m_equalityFunction = _keysEqualFunc;
    hash->m_hashSize = _capacity;
    hash->m_capacity = size;
    hash->m_numOfItems = 0;
    hash->m_magic = MAGIC;
    return hash;
}

void HashMapDestroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value)){
    int i;
    Data* element;
    if(NULL != _map && (*_map)->m_magic == MAGIC){
        if((*_map)->m_numOfItems > 0 && NULL != (*_map)->m_data){
            for(i = 0; i < (*_map)->m_capacity; i++){
                element = &i;
                if((*_map)->m_data[i] != NULL){
                    DestroyDataInList(_map, _keyDestroy, _valDestroy);
                    ListDestroy(&(*_map)->m_data[i], NULL);
                }
            }
        }
        (*_map)->m_magic = 0;
        if(NULL != (*_map)->m_data)
            free((*_map)->m_data);
        free(*_map);
    }
}

MapResult HashMapInsert(HashMap* _map, const void* _key, const void* _value){
    size_t index;
    Data *d;
    List *list;
    if(NULL == _map || NULL == _key){
        return MAP_UNINITIALIZED_ERROR;
    }
    d = (Data*)malloc(sizeof(Data));
    if(NULL == d)
        return MAP_ALLOCATION_ERROR;
    d->m_key = _key;
    d->m_value = _value;
    index = _map->m_hashFunction(_key) % _map->m_capacity;
    if(_map->m_data[index] == NULL){
        list = ListCreate();
        _map->m_data[index] = list;
    }
    if(Iteration(_map, _key) != NULL)
        return MAP_KEY_DUPLICATE_ERROR;
    if(ListPushHead(_map->m_data[index], d) != LIST_SUCCESS)
        return MAP_ALLOCATION_ERROR;
    _map->m_numOfItems += 1;
    return MAP_SUCCESS;
}

MapResult HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue){
    void *node1, *node2;
    Data *d;
    int index;
    if(NULL == _map || NULL == _pKey || NULL == _pValue){
        return MAP_UNINITIALIZED_ERROR;
    }
    if(NULL == _searchKey)
        return MAP_KEY_NULL_ERROR;
    if(_map->m_numOfItems == 0)
        return MAP_KEY_NOT_FOUND_ERROR;
    index = _map->m_hashFunction(_searchKey) % _map->m_capacity;
    node1 = ListItrBegin(_map->m_data[index]); /* gets a pointer to the begin of the list */
    node2 = ListItrEnd(_map->m_data[index]); /* gets a pointer to the end of the list */
    d = ListItrGet(node1);
    if(NULL != d && _map->m_equalityFunction(d->m_key, _searchKey) == 1){
        _pKey = d->m_key;
        _pValue = d->m_value;
        ListItrRemove(node1);
        return MAP_SUCCESS;
    }
    while(ListItrEquals(node1, node2) == 0){
        node1 = ListItrNext(node1);
        d = ListItrGet(node1);
        if(NULL != d && _map->m_equalityFunction(d->m_key, _searchKey) == 1){
            _pKey = d->m_key;
            _pValue = d->m_value;
            ListItrRemove(node1);
            return MAP_SUCCESS;
        }
    }
    return MAP_KEY_NOT_FOUND_ERROR;
}

MapResult HashMapFind(const HashMap* _map, const void* _key, void** _pValue){
    Data *d;
    if(NULL == _map || NULL == _pValue)
        return MAP_UNINITIALIZED_ERROR;
    if(NULL == _key)
        return MAP_KEY_NULL_ERROR;
    d = Iteration(_map, _key);
    if(NULL != d){
        *_pValue = d->m_value;
        return MAP_SUCCESS;
    }
    return MAP_KEY_NOT_FOUND_ERROR;
}

size_t HashMapSize(const HashMap* _map){
    if(NULL != _map)
        return _map->m_numOfItems;
    return 0;
}

size_t HashMapForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context){
    void *node1, *node2;
    Data *d;
    int i, count = 0;
    if(NULL == _map || NULL == _context || _map->m_numOfItems == 0){
        return 0;
    }
    for(i = 0; i < _map->m_capacity; i++){
        node1 = ListItrBegin(_map->m_data[i]); /* gets a pointer to the begin of the list */
        node2 = ListItrEnd(_map->m_data[i]); /* gets a pointer to the end of the list */
        d = ListItrGet(node1);
        if(NULL != d){
            if(_action(d->m_key, d->m_value, _context) == 0){
                count++;
                return count;
            }
            count++;
        }
        while(ListItrEquals(node1, node2) == 0){
            node1 = ListItrNext(node1);
            d = ListItrGet(node1);
            if(NULL != d){
                if(_action(d->m_key, d->m_value, _context) == 0){
                    count++;
                    return count;
                }
                count++;
            }
        }
    }
    return count;
}

/*  
 *  This function calculate the actual size of the hash table (it's different than 
 *  the user's requested size).
 *  It takes the closet Prime number to _size * MARGIN.
 */
static int Capacity(const int _size){
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

/* Returns data if key is found, otherwise returns NULL */
static Data* Iteration(const HashMap* _hash, void* _key){
    void *node1, *node2;
    Data *d;
    int index;
    if(NULL == _hash || NULL == _key || _hash->m_numOfItems == 0){
        return NULL;
    }
    index = _hash->m_hashFunction(_key) % _hash->m_capacity;
    node1 = ListItrBegin(_hash->m_data[index]); /* gets a pointer to the begin of the list */
    node2 = ListItrEnd(_hash->m_data[index]); /* gets a pointer to the end of the list */
    d = ListItrGet(node1);
    if(NULL != d && _hash->m_equalityFunction(d->m_key, _key) == 1)
        return d;
    while(ListItrEquals(node1, node2) == 0){
        node1 = ListItrNext(node1);
        d = ListItrGet(node1);
        if(NULL != d && _hash->m_equalityFunction(d->m_key, _key) == 1)
            return d;
    }
    return NULL;
}

static void DestroyDataInList(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value)){
    if(NULL != _map){
        if(NULL != _keyDestroy || NULL != _valDestroy){
            while(ListPopHead((*_map)->m_data[i], &element) != LIST_UNDERFLOW_ERROR){
                if(NULL != element){
                    _keyDestroy(element->m_key);
                    _valDestroy(element->m_value);
                }
            }
        }
    }
}