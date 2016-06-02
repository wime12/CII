#ifndef NTREE_INCLUDED
#define NTREE_INCLUDED

#define T NTree_T

typedef struct T *T;

/**
 * NTrees represent structured data.
 *
 * Each node in an NTree can have siblings and children.
 *
 * The empty tree is just NULL. The first data has to be entered in
 * a tree using `NTree_insert_before` or `NTree_insert_after`, which
 * behave the same in this case.
 */

/**
 * @brief Copies an tree.
 *
 * Create a copy of a tree. If `copy_data` is NULL then a
 * shallow copy is created, otherwise also the data is copied.
 *
 * @param tree The original tree
 * @param copy_data The function to copy the data
 * @param cl Data passed unaltered to `copy_data`
 *
 * @return A deep or shallow copy of the tree
 *
 * @throw Mem_Failed
 */
extern T NTree_copy(const T tree,
    void *(*copy_data)(const void *data, void *cl), void *cl);

/**
 * @brief Free a tree.
 *
 * Release the tree and, if `free_data` is not NULL, also the data.
 * The root of the tree is set to NULL
 *
 * @param treep A pointer to the root of the tree
 * @param free_data The function to free the data
 * @param cl Data passed unchanged to `free_data`
 */
extern void NTree_free(T *treep,
    void *(free_data)(void *data, void *cl), void *cl);

/**
 * Tests whether a tree is empty.
 *
 * @param tree The root of the tree
 *
 * @return 1 if the tree is empty, 0 otherwise.
 */
extern unsigned int NTree_is_empty(const T tree);

/**
 * Calculates the size of a tree.
 *
 * @param tree The root of the tree
 *
 * @return The number of entries in the tree.
 */
extern unsigned int NTree_size(const T tree);

/**
 * @brief Tests if an entry occurs in a tree.
 *
 * An entry is searched in the tree by comparing some data to
 * the data of the entries using a comparison function. As soon
 * as the comparison function returns 0 this function returns 1.
 * If no entry was found it returns 0.
 *
 * @param tree The root of the tree
 * @param data The data with which each entry is compared
 * @param comp The comparison function
 * @param cl Data passed unchanged to comp
 *
 * @return 1 if an entry was found, 0 otherwise.
 */
extern int NTree_occurs(const T tree, const void *data,
    int (*comp)(const void *data1, const void *data2, void *cl), void *cl);

/**
 * @brief Returns the number of occurrances of an entry in a tree.
 *
 * The number of occurrances of an entry is searched in the tree by
 * comparing the given data to the data of the entries using a comparison
 * function. The count is increased if the comparison function returns 0.
 *
 * @param tree The root of the tree
 * @param data The data with which each entry is compared
 * @param comp The comparison function
 * @param cl Data passed unchanged to comp
 *
 * @return The number of occurrances
 */
extern unsigned int NTree_occurrances(const T tree, const void *data,
    int (*comp)(const void *data1, const void *data2, void *cl), void *cl);

/**
 * @brief Apply a function to the data of each entry in a tree.
 *
 * @param tree The root of the tree
 * @param apply The function to apply
 * @param cl Data passed unchanged to the applied function
 */
extern void NTree_traverse(T tree,
    int (*apply)(void **datap, void *cl), void *cl);

// Movement

/**
 * @brief Get the first sibling.
 * 
 * Get the first sibling of all siblings of this entry.
 *
 * @param tree The tree
 *
 * @return The first sibling or NULL if there is none.
 */
extern T NTree_first(const T tree);

/**
 * @brief Get the next sibling of the current entry.
 *
 * Get the next neighboring sibling of this entry.
 *
 * @param tree The tree
 *
 * @return The next sibling or NULL if there is none.
 */
extern T NTree_next(const T tree);

/**
 * @brief Get the previous sibling.
 *
 * Get the previous neighboring sibling of this entry.
 *
 * @param tree The tree
 *
 * @return The previous sibling or NULL if there is none.
 */
extern T NTree_previous(const T tree);

/**
 * @brief Get the last sibling.
 *
 * Get the last of all siblings of this entry.
 *
 * @param tree The tree
 *
 * @return The last sibling or NULL if there is none.
 */
extern T NTree_last(const T tree);

/**
 * @brief Get the next occurrance of an entry.
 *
 * Get the next occurrance of an entry. The current position
 * is also checked. The position is found if the comparison function
 * returns 0.
 *
 * @param tree The tree
 * @param cmp The comparison function
 * @param cl Data passed unchanged to the comparison function
 *
 * @return The next occurrance or NULL if there is none.
 */
extern T NTree_find(const T tree, const void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl),
    void *cl);

/**
 * @brief Get the root of the tree.
 *
 * @param tree The tree
 *
 * @return The root of the tree or NULL if the tree is empty.
 */
extern T NTree_root(const T tree);

/**
 * @brief Get the first child.
 *
 * @param tree The tree
 *
 * @return The first child or NULL if there is none.
 */
extern T NTree_first_child(const T tree);

// Tests

/**
 * @brief Tests whether this is the first sibling.
 *
 * @param tree The tree
 *
 * @return 1 if this is the first sibling, 0 otherwise
 */
extern int NTree_on_first(const T tree);

/**
 * @brief Tests whether this is the last sibling.
 *
 * @param tree The tree
 *
 * @return 1 if is the last sibling, 0 otherwise.
 */
extern int NTree_on_last(const T tree);

/**
 * @brief Tests whether the cursor is on the root of the tree.
 *
 * @param tree The tree
 *
 * @return 1 if this is the root of the tree, 0 otherwise.
 */
extern int NTree_on_root(const T tree);

/**
 * @brief Tests if there are children.
 *
 * @param tree The tree
 *
 * @return 1 if this parent has children, 0 otherwise.
 */
extern int NTCursor_has_children(const T tree);

// Data

/**
 * @brief Get the data of the current entry.
 *
 * Get the data of the current entry. It is an error to
 * call this function on an empty tree.
 *
 * @param tree The tree
 *
 * @return The data
 */
extern void *NTree_data(const T tree);

/**
 * @brief Set the data of the current entry and return the old data.
 *
 * Set the data of the entry at the current position and
 * return the old data. It is an error to call this function
 * on an empty tree.
 *
 * @param tree The tree
 * @param data The new data
 *
 * @return The old data or NULL if there was none
 */
extern void *NTree_set_data(T tree, const void *data);

// Mutation

/**
 * @brief Prepend a child
 *
 * Insert a child at the start of the list of children of the current
 * position. It is an error to call this function on an empty tree.
 *
 * @param tree The tree
 * @param data The data of the new child
 *
 * @throw Mem_Failed
 */
extern void NTree_prepend_child(T tree, const void *data);

/**
 * @brief Append a child
 *
 * Insert a child at the end of the list of children of the current
 * position.
 *
 * @param tree The tree
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTree_append_child(T tree, const void *data);

/**
 * @brief Insert a sibling before the current position.
 *
 * @param tree The tree
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTree_insert_before(T tree, void *data);

/**
 * @brief Insert a sibling after the current position.
 *
 * @param cursor The cursor
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTree_insert_after(T tree, void *data);

/**
 * @brief Remove the entry at the current position
 *
 * Remove the entry at the current position of the cursor if it has
 * no children and free its memory (but not the memory of the data).
 *
 * The value returned is
 *   1) the next sibling if there is one or
 *   2) the previous sibling if there is no next sibling but a
 *      previous one or
 *   3) the parent if there are no siblings left.
 *
 * @param treep A pointer to the current position, will be set to NULL.
 *
 * @return The new position
 */
extern T NTree_remove(T *treep);

#undef T

#endif
