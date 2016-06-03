#ifndef NTREE_INCLUDED
#define NTREE_INCLUDED

#define T NTree_T

typedef struct T *T;

typedef void *(*NTree_copy_data_fun_T)(const void *data, void *cl);

typedef void *(*NTree_free_data_fun_T)(void *data, void *cl);

typedef int (*NTree_compare_fun_T)(const void *data1, const void *data2, void *cl);

typedef void (*NTree_apply_fun_T)(void **datap, void *cl);

/**
 * NTrees represent structured data.
 *
 * Each child or sibling in a tree is another subtree.
 *
 * The empty tree is just NULL. The first data has to be entered in
 * a tree using `NTree_insert_before` or `NTree_insert_after`, which
 * behave the same in this case.
 *
 * The root of the current subtree is just called "root". If the root of 
 * a tree does not have a predecessor, i.e. it has neither preceding siblings
 * nor a parent, then it is called an "absolute root" and the tree starting
 * at this absolute root is called the "whole tree".
 *
 * A root may have children and following siblings. Preceding siblings
 * are not part of the subtree which starts at root.
 *
 * In the documentation to the function there is made no distinction
 * between whole tree and subtree. "Tree" is used for both terms
 * interchangeably.
 */

/**
 * @brief Copy a tree.
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
extern T NTree_copy(const T tree, NTree_copy_data_fun_T copy_data,
    void *cl);

/**
 * @brief Free a tree.
 *
 * Release the tree and, if `free_data` is not NULL, also the data.
 * The root of the tree is set to NULL and the subtree is cut off
 * from the whole tree.
 *
 * Free a whole tree by first finding the absolute root and calling
 * this function on it.
 *
 * @param treep A pointer to the root of the tree
 * @param free_data The function to free the data
 * @param cl Data passed unchanged to `free_data`
 */
extern void NTree_free(T *treep, NTree_free_data_fun_T free_data,
    void *cl);

/**
 * @brief Calculate the size of a tree.
 *
 * @param tree The root of the tree
 *
 * @return The number of entries in the tree.
 */
extern unsigned int NTree_size(const T tree);

/**
 * @brief Get the number of occurrances of an entry in a tree.
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
    NTree_compare_fun_T cmp, void *cl);

/**
 * @brief Apply a function to each data entry in a tree.
 *
 * Apply a function to each data entry in the tree. The order of the
 * traversal is unspecified.
 *
 * @param tree The root of the tree
 * @param apply The function to apply
 * @param cl Data passed unchanged to the applied function
 */
extern void NTree_traverse(T tree, NTree_apply_fun_T apply, void *cl);

// Movement

/**
 * @brief Get the first sibling of this root.
 * 
 * Get the first sibling in the list of siblings.
 *
 * @param tree The root of a tree
 *
 * @return The first sibling or NULL if there is none.
 */
extern T NTree_first(const T tree);

/**
 * @brief Get the next sibling of this root.
 *
 * Get the next neighboring sibling.
 *
 * @param tree The root of a tree
 *
 * @return The next sibling or NULL if there is none.
 */
extern T NTree_next(const T tree);

/**
 * @brief Get the previous sibling of this root.
 *
 * Get the previous neighboring sibling of this root.
 *
 * @param tree The root of a tree
 *
 * @return The previous sibling or NULL if there is none.
 */
extern T NTree_previous(const T tree);

/**
 * @brief Get the last sibling of this root.
 *
 * Get the last sibling in the list of siblings.
 *
 * @param tree The root of a tree
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
 * The children are checked first then the following siblings.
 *
 * @param tree The root of a tree
 * @param cmp The comparison function
 * @param cl Data passed unchanged to the comparison function
 *
 * @return The next occurrance or NULL if there is none.
 */
extern T NTree_find(const T tree, const void *data,
    NTree_compare_fun_T cmp, void *cl);

/**
 * @brief Get the absolute root of the tree.
 *
 * @param tree The root of a tree
 *
 * @return The root of the tree or NULL if the tree is empty.
 */
extern T NTree_absolute_root(const T tree);

/**
 * @brief Get the first child.
 *
 * @param tree The root of a tree
 *
 * @return The first child or NULL if there is none.
 */
extern T NTree_first_child(const T tree);

// Tests

/**
 * @brief Test whether this root is the first one in the list of siblings.
 *
 * @param tree The root of a tree
 *
 * @return 1 if this is the first sibling, 0 otherwise
 */
extern int NTree_is_first(const T tree);

/**
 * @brief Test whether this root is the last one in the list of siblings.
 *
 * @param tree The root of a tree
 *
 * @return 1 if is the last sibling, 0 otherwise.
 */
extern int NTree_is_last(const T tree);

/**
 * @brief Test whether this is an absolute root.
 *
 * @param tree The root of a tree
 *
 * @return 1 if this is the root of the tree, 0 otherwise.
 */
extern int NTree_is_absolute_root(const T tree);

/**
 * @brief Test if this root has children.
 *
 * @param tree The root of a tree
 *
 * @return 1 if there are children, 0 otherwise.
 */
extern int NTCursor_has_children(const T tree);

// Data

/**
 * @brief Get the data of the root.
 *
 * Get the data of the root. It is an error to
 * call this function on an empty tree.
 *
 * @param tree The root of a tree
 *
 * @return The data
 */
extern void *NTree_data(const T tree);

/**
 * @brief Set the data of the root and return the old data.
 *
 * Set the data of the root and return the old data. It is an
 * error to call this function on an empty tree.
 *
 * @param tree The root of a tree
 * @param data The new data
 *
 * @return The old data or NULL if there was none
 */
extern void *NTree_set_data(T tree, void *data);

// Mutation

/**
 * @brief Prepend a child
 *
 * Insert a child at the start of the list of children of the current
 * position. It is an error to call this function on an empty tree.
 *
 * @param tree The root of a tree
 * @param data The data of the new child
 *
 * @throw Mem_Failed
 */
extern void NTree_prepend_child(T tree, void *data);

/**
 * @brief Append a child
 *
 * Insert a child at the end of the list of children of the current
 * position.
 *
 * @param tree The root of a tree
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 */
extern void NTree_append_child(T tree, void *data);

/**
 * @brief Insert a sibling before the current position.
 *
 * @param tree The tree
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 *
 * @return The newly inserted entry.
 */
extern T NTree_insert_before(T tree, void *data);

/**
 * @brief Insert a sibling after the current position.
 *
 * @param cursor The cursor
 * @param data The data of the new sibling
 *
 * @throw Mem_Failed
 *
 * @return The newly inserted entry
 */
extern T NTree_insert_after(T tree, void *data);

/**
 * @brief Remove the entry at the current position
 *
 * Remove the entry at the current position of the cursor if it has
 * no children and free its memory (but not the memory of the data).
 *
 * The returned value is
 *   0) NULL if this entry was not removed 
 *   1) the next sibling if there is one or
 *   2) the previous sibling if there is no next sibling but a
 *      previous one or
 *   3) the parent if there are no siblings left.
 *
 * The pointer to the entry is set to NULL if the entry was removed
 * or it stays unchanged.
 *
 * @param tree A pointer to the root of a tree
 *
 * @return The new position
 */
extern T NTree_remove(T *treep);

#undef T

#endif
