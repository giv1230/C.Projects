#ifndef __HASHSET_H__
#define __HASHSET_H__

# include <stddef.h>  /* size_t */

typedef struct Hash Hash;



typedef enum HashSet_Result {
    SET_SUCCESS = 0,
    SET_UNINITIALIZED,         /** Uninitialized HasSeterror*/
    SET_KEY_DUPLICATE,         /** Duplicate key error */
    SET_KEY_NOT_FOUND,         /** Key not found */
    SET_UNDERFLOW,         /* No items to remove from th hash */
    SET_OVERFLOW         /**No more space in HashSe */
}HashSet_Result;

/** 
 * @brief A function which it uses to calculate the index in which the _data
 *        will be inserted to.
 *
 * @param _data : the data to be inserted
 * @return positive number
 */
typedef size_t (*HashFunction)(void* _data);

/** 
 * @brief Equality function returns 1 if the elements are equal and 0 otherwise.
 *
 * @param _firstData : first element to compare
 * @param _secondData : second element to compare
 * @return 1 if equal, 0 if not equal
 */
typedef int (*EqualityFunction)(void* _firstData, void* _secondData);


/** 
 * @brief Create a Hash Set Table
 * Creates an empty Hash Set Table
 * 
 * @param _size: the HashSet Table size, must be bigger than one.
 * @param _hashFunction A function which it uses to calculate the index in which the _data
 *                      will be inserted to.
 * @param _equalityFunction: A comparison function 
 *				             that returns 1 (none zero value) if x = y 
 *				             and 0 (zero) otherwise.
 * @return a pointer to the newly created tree.
 * @retval NULL due to allocation failure or due to _size < 2, otherwise it returns 
 *         pointer to the created HashSet table 
 */
Hash* HashCreate(size_t _size, size_t (*_hashFunction)(void*), int (*_equalityFunction)(void*, void*));


/** 
 * @brief Destroy Hash Table
 * Destroys the Hash Table, freeing the memory.
 * If supplied with non-NULL keyDestroy function, frees items in Hash Table.
 * Average time complexity: O(1)..O(n) (depends on memory freeing)
 *
 * @param _hash : A previously created Hash Table returned via HashCreate.
 * @param _keyDestroy : A function to destroy the data in the Hash Table (may be NULL if unnecessary)
 */
void HashDestroy(Hash** _hash, void (*_keyDestroy)(void* _data));


/** 
 * @brief Add an element to Hash Table if it's not already there
 * Insert element to  Hash Table, using the Hash Table's EqualityFunction function
 * Average time complexity O(1).
 * Worst: O(1)
 *
 * @param _hash : A previously created Hash Table returned via HashCreate
 * @param _data: An item to add to the Hash Table
 * @return HashSet_Result - 
 * @retval SET_SUCCESS - on seccess
 * @retval SET_UNINITIALIZED - if _hash or _data is NULL
 * @retval SET_KEY_DUPLICATE - if there is already similar _item in the hash table.
 * @retval SET_OVERFLOW - if there is no more room to add 
 * @warning don't allow duplicates
 */
HashSet_Result HashInsert(Hash* _hash, void* _data);


/**
 * @brief Remove the given key from the hash table 
 * @param _hash - the hash to remove the information from
 * @param _key - A pointer to the information to be removed
 * @param _data - a pointer to the removed key
 * @return HashSet_Result -
 * @retval SET_SUCCESS - on seccess
 * @retval SET_UNINITIALIZED - if _hash is NULL
 * @retval SET_UNDERFLOW - if the hash is empty.
 * @retval SET_KEY_NOT_FOUND - if the key was not found.
 */
HashSet_Result HashRemove(Hash* _hash, void* _key, void** _data);


/**
 * @brief Search for a given key, in the hash table 
 * @param _hash - the hash that holds the information
 * @param _key - A pointer to the information to be found
 * @return 1 - if the key was found and 0 otherwise
 * @retval 1 - on seccess
 * @retval 0 - if the key was not found.
 */
int HashIsFound(const Hash* _hash, void* _key);


/**
 * @brief returns the number of keys currently in the hash table 
 * @param _hash - the hash that holds the information
 * @return 0 if there are no keys in the hash table or if the given _hash is NULL
 *         none zero value if the hash is not empty
 * @retval positive - num of items in the hash table
 * @retval 0 - if there are no keys in the hash table or if the given _hash is NULL.
 */
size_t HashNumOfItems(const Hash* _hash);


/**
 * @brief returns the hash table size.
 * @param _hash - the hash that holds the information
 * @return 0 if _hash is NULL and none zero value if the hash is not NULL.
 * @retval positive - the size of the hash table
 * @retval 0 - if the given _hash is NULL.
 */
size_t HashCapacity(const Hash* _hash);


/**
 * @brief returns the avarage amount of rehashes that was made in each insertion till now.
 * @param _hash - the hash that holds the information
 * @return 0 if _hash is NULL and none zero value if the hash is not NULL.
 * @retval positive - the avarage amount of rehashes that was made in each insertion till now.
 * @retval 0 - if the given _hash is NULL.
 */
double HashAverageRehashes(const Hash* _hash);


/**
 * @brief returns the maximal amount of rehashes that was made in one insertion till now.
 * @param _hash - the hash that holds the information
 * @return 0 if _hash is NULL and none zero value if the hash is not NULL.
 * @retval positive - the maximal amount of rehashes that was made in one insertion till now.
 * @retval 0 - if the given _hash is NULL.
 */
size_t HashMaxReHash(const Hash* _hash);


/**
 * @brief prints all the keys in the Hash Table.
 * @param _hash - the hash that holds the information.
 * @param PrintKey - a function that prints each individual key.
 * @return void - no return value.
 */
void HashPrint(const Hash* _hash, void (*PrintKey)(void*));


#endif /* __HASHSET_H__ */