#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <stddef.h>

#define T Map_T

typedef int (*Map_cmp_fun_T)(const void *, const void *);
typedef void *(*Map_copy_fun_T)(const void *);
typedef void (*Map_free_fun_T)(void *);
typedef void (*Map_apply_fun_T)(const void *, const void *, void *);

typedef struct T *T;

/**
 * @brief Creates a new map.
 *
 * Creates a new empty map. A map is a collection of key-value
 * associations ordered by their keys. The comparison function for
 * the keys is a required parameter. All others can be NULL. (See Map_copy
 * and Map_free for a description.)
 *
 * The comparison function determins the order of the keys in the map.
 * It must return a number < 0 if the first key is smaller than the
 * second one, a number > 0 if the first key is greater than the second
 * one and 0 if the keys are equal.
 *
 * @param cmp The comparison function for keys.
 * @param copy_key The copy function for keys. May be NULL.
 * @param copy_data The copy function for data. May be NULL.
 * @param free_key A function releasing the memory occupied by the key. May be NULL
 * @param free_data A function releasing the memory occupied by the data. May be NULL
 *
 * @return A new empty map.
 *
 * @throw Mem_failed
 */
extern T Map_new(Map_cmp_fun_T cmp,
    Map_copy_fun_T copy_key, Map_copy_fun_T copy_data,
    Map_free_fun_T free_key, Map_free_fun_T free_data);

/**
 * @brief Create a copy of the map
 *
 * Create a copy of the map. If the copy functions for keys and
 * data were given during the map creation, then also the keys
 * and the data is copied (deep copy). A missing copy function means
 * that the item itself and not a copy is transferred to the new map.
 *
 * This function has space complexity O(log(n)) and time complexity
 * O(n) where n is the number of elements in the original map.
 *
 * @param map The map
 *
 * @return A new map containing all associations of the old one.
 *
 * @throw Mem_failed
 */
extern T Map_copy(const T map);

/**
 * @brief Frees the map
 *
 * The memory used by map is released and the map is set to NULL.
 * If functions for freeing the keys and the data were given
 * during the map creation, also the keys and the data are freed.
 *
 * @param A pointer to the map
 */
extern void Map_free(T *mapp);

/**
 * @brief Associate data with a key in map
 *
 * Inserts data associated with a key in the map. If the key
 * already exists in the map nothing is inserted, the old
 * association is retained and the function returns 0. Otherwise
 * the function returns 1.
 *
 * This function has space complexity O(1) and time complexity
 * O(log(n)) where n is the number of elements in the map.
 *
 * @param map The map
 * @param key The key
 * @param data The data
 *
 * @return 1 on success and 0 if key exists already
 *
 * @throws Mem_failed
 */
extern int Map_insert(T map, const void *key, const void *data);

/**
 * @brief removes an association from the map
 *
 * The data associated with the key and the key itself are removed
 * from the map and removed data is returned. If the key does not exist
 * in the map nothing is removed and the return value is NULL.
 *
 * This function has space complexity O(1) and time complexity
 * O(log(n)) where n is the number of elements in the map.
 *
 * @param map The map
 * @param key The key
 *
 * @return The removed data or NULL
 */
extern const void *Map_remove(T map, const void *key);

/**
 * @brief Get the data associated with the key from the map.
 *
 * Retrieve the data associated with the key from the map.
 *
 * This function has space complexity O(1) and time complexity
 * O(log(n)) where n is the number of elements in the map.
 *
 * @param map The map
 * @param key The key
 *
 * @return The data associated with `key`
 */
extern const void *Map_get(const T map, const void *key);

/**
 * @brief apply a function to all elements of the tree in order
 *
 * Applies a function to all elements of the tree in order
 * starting with the smallest one. The last parameter is passed unchanged
 * to the function on each invocation.
 *
 * The traversal has space complexity O(1) and time complexity O(n)
 * where n is the number of entries in the map.
 *
 * @param map The map
 * @param apply The function to apply on each element
 * @param cl Data passed to `apply` on each invocation
 */
extern void Map_traverse(const T map, const Map_apply_fun_T apply, void *cl);

/**
 * @brief Return the number of entries in the map
 *
 * Returns the number of entries in the map.
 *
 * This function has space complexity O(1) and time complexity O(n)
 * where n is the number of entries in the map.
 *
 * @param The map
 *
 * @return The number of entries
 */
extern size_t Map_size(const T map);

#undef T

#endif
