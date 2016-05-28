#ifndef NTREE_INCLUDED
#define NTREE_INCLUDED

#define T NTree_T

typedef struct T *T;

/**
 * NTrees represent structured data.
 *
 * Each node in an NTree can have siblings and children.
 *
 * The empty tree is just NULL. Data can be entered in a tree by
 * setting a NTCursor on a tree and by then using the functions
 * `NTCursor_insert_before`, `NTCursor_insert_after`,
 * `NTCursor_prepend_child` and `NTCursor_append_child`. Only the
 * `insert` functions can be used on empty trees. They behave the
 * same on empty trees.
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
extern int NTree_occurs(const T tree, void *data,
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
extern unsigned int NTree_occurrances(const T tree, void *data,
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

#undef T

#define T NTCursor_T

typedef struct T *T;

/**
 * Make a new cursor and connect it to a tree.
 *
 * @param tree The root of the tree
 *
 * @return A new cursor
 *
 * @throw Mem_Failed
 */
extern T NTCursor_new(NTree_T tree);

// Movement

/**
 * Ensures that the cursor is on the first sibling of the current
 * position.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 */
extern void NTCursor_first(T cursor);

/**
 * Move to the next sibling of the current position.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 *
 * @return 1 if there is a next sibling, 0 otherwise.
 */
extern int NTCursor_next(T cursor);

/**
 * Move to the previous sibling of the current position.
 *
 * @param cursor The cursor
 *
 * @return 1 if there is a previous sibling, 0 otherwise.
 */
extern int NTCursor_previous(T cursor);

/**
 * Ensures that the cursor is on the last sibling.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 */
extern void NTCursor_last(T cursor);

/**
 * @brief Move the cursor to the occurrance of an entry.
 *
 * Move the cursor to the position of the next occurrance of
 * data. The current position is also checked. The position is found
 * if the comparison function returns 0.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 * @param cmp The comparison function
 * @param cl Data passed unchanged to the comparison function
 *
 * @return 1 if the next occurrance was found, 0 otherwise
 */
extern int NTCursor_find(T cursor, void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl),
    void *cl);

/**
 * Ensures that the cursor is on the root of the tree.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 */
extern void NTCursor_root(T cursor);

/**
 * Move the cursor to the first child of this position.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 *
 * @return 1 if there is a child, 0 otherwise.
 */
extern int NTCursor_first_child(T cursor);

// Tests

/**
 * Tests whether the cursor is on the first sibling.
 *
 * @param cursor The cursor
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @return 1 if the cursor is on the first sibling, 0 otherwise
 */
extern int NTCursor_on_first(T cursor);

/**
 * Tests whether the cursor is on the last sibling.
 *
 * Must not be called if the cursor is set to an empty tree.
 * 
 * @param cursor The cursor
 *
 * @return 1 if the cursor is on the last sibling, 0 otherwise.
 */
extern int NTCursor_on_last(T cursor);

/**
 * Tests whether the cursor is on the root of the tree.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 *
 * @return 1 if the cursor is on the root of the tree, 0 otherwise.
 */
extern int NTCursor_on_root(T cursor);

/**
 * Tests if the current position of the cursor has children.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 *
 * @return 1 if this parent has children, 0 otherwise.
 */
extern int NTCursor_has_children(T cursor);

// Data

/**
 * Get the data of the current position.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 *
 * @return The data
 */
extern void *NTCursor_get(T cursor);

/**
 * Set the data at the current position and return the old data.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 * @param data The new data
 *
 * @return The old data
 */
extern void *NTCursor_set(T cursor, void *data);

// Mutation

/**
 * @brief Prepend a child
 *
 * Insert a child at the start of the list of children of the current
 * position. The cursor stays at the current position, i.e. at the parent.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 * @param data The data of the new child
 *
 * @throw Mem_Failed
 */
extern void NTCursor_prepend_child(T cursor, void *data);

/**
 * @brief Append a child
 *
 * Insert a child at the end of the list of children of the current
 * position. The cursor stays at the current position, i.e. at the parent.
 *
 * @param cursor The cursor
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTCursor_append_child(T cursor, void *data);

/**
 * @brief Prepend a sibling
 *
 * Insert a sibling before the current position of the cursor.
 * The cursor stays at the current position.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTCursor_insert_before(T cursor, void *data);

/**
 * @brief Append a sibling
 *
 * Insert a sibling after the current position of the cursor.
 * The cursor stays at the current position.
 *
 * @param cursor The cursor
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTCursor_insert_after(T cursor, void *data);

/**
 * @brief Remove the entry at the current position
 *
 * Remove the entry at the current position of the cursor if it has
 * no children. The data of the current position is returned if the node
 * was removed or NULL if it could not be removed.
 * The cursor moves to the next sibling if there is one or
 * to the previous sibling if there is no next sibling but a
 * previous one or to the parent if there are no siblings left.
 *
 * Must not be called if the cursor is set to an empty tree.
 *
 * @param cursor The cursor
 */
extern void *NTCursor_remove(T cursor);

#undef T

#endif
