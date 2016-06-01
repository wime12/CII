#ifndef RBTREE_INCLUDED
#define RBTREE_INCLUDED

#define T RBTree_T

typedef struct T *T;

typedef void *(*RBTree_copy_data_fun_T)(const void *data, void *cl);

typedef void (*RBTree_free_data_fun_T)(void *data, void *cl);

typedef int (*RBTree_compare_fun_T)(const void *data1,
    const void *data2, void *cl);

typedef void (*RBTree_apply_fun_T)(const void *data, void *cl);

/**
 * @brief Makes a new node initialized with data.
 *
 * A new node is created, which contains data `x`.
 *
 * @param x The data content of the node
 *
 * @return a new node
 *
 * @throw Mem_Failed
 */
extern T RBTree_new(const void *data);

/**
 * @brief Copies a tree
 *
 * Copies a tree. If the function pointer copy_data is NULL then
 * a shallow copy is created and both trees share the same data.
 *
 * @param tree The root of the tree
 * @param copy_data A function pointer to copy the data
 * @param cl Is passed unaltered to copy_data
 *
 * @return A new tree
 *
 * @throw Mem_Failed
 */
extern T RBTree_copy(const T tree, RBTree_copy_data_fun_T copy_data, void *cl);

/**
 * @brief Frees a tree.
 *
 * Free the memory occupied by the tree and set the
 * tree to NULL. If the `free_data` parameter is not NULL
 * then also the data in the tree will be freed. `cl` is
 * passed unaltered to `free_data`.
 *
 * @param treep A pointer to the tree
 * @param free_data A function used to free the data.
 * @param cl Passed unaltered to `free_data`
 */
extern void RBTree_free(T *treep, RBTree_free_data_fun_T free_data, void *cl);

/**
 * @brief Inserts data into the tree
 *
 * Insert data into the tree preserving the ordering prescribed
 * by a comparison function. The root of the tree may be altered.
 * The comparison function returns an integer < 0 if data1 is
 * smaller data2, 0 if data1 is equal to data2 and an integer > 0
 * if data1 is bigger than data2.
 *
 * If the data was not inserted because one entry was equal to the 
 * new data, then the old entry is retained and the function returns
 * 0. If the data was inserted the function returns 1.
 *
 * @param treep Pointer to the root of the tree, contains the new root of the tree after the insertion.
 * @param data Pointer to the data
 * @param cmp Comparison function
 * @param cl pointer to data which will be passed unchanged to cmp
 *
 * @return 1 if node was inserted 0 if not
 *
 * @throw Mem_Failed
 */
extern int RBTree_insert(T *treep, const void *data, RBTree_compare_fun_T cmp,
    void *cl);

/**
 * @brief Removes data from the tree
 *
 * Removes an entry from the tree if the comparison function `cmp`
 * called on the given data and the data of the entry return 0. The
 * function returns the removed data or NULL if nothing was removed.
 * `treep` contains the new root of the tree after the function finishes.
 *
 * @param treep A pointer to the root of the tree.
 * @param data A pointer to the data to be compared with the entries
 * @param cmp The comparison function
 * @param cl Data passed unchanged to cmp
 *
 * @return NULL if nothing was removed or the removed data
 */
extern const void *RBTree_remove(T *treep, const void *data,
    RBTree_compare_fun_T cmp, void *cl);

/**
 * @brief Get data from the tree
 *
 * Get data from the tree entry for which `cmp` applied on the given
 * data and the data of the entry returns 0.
 *
 * @param tree The root of the tree
 * @param data The data to be compared with the data of the tree entries
 * @param cmp The comparison function
 * @param cl Data passed unchanged to `cmp`
 *
 * @return The required data
 */
extern const void *RBTree_get(const T tree, const void *data,
    RBTree_compare_fun_T cmp, void *cl);

/**
 * @brief Applies a function to all entries in the tree in order.
 *
 * The function `apply` is called on each data entry in the tree in
 * increasing order.
 *
 * @param tree The root of the tree
 * @param apply The function to be applied on each entry
 * @param cl Data passed unchanged to `apply`
 */
extern void RBTree_traverse(T tree, RBTree_apply_fun_T apply, void *cl);

/**
 * Calculate the number of entries in the tree.
 *
 * @param tree The root of the tree
 *
 * @return The number of entries in the tree.
 */
extern unsigned int RBTree_size(const T tree);

/**
 * Calculate the depth of the tree.
 *
 * @param tree The root of the tree
 *
 * @return The depth of the tree
 */
extern unsigned int RBTree_depth(const T tree);

#undef T

#endif
