#ifndef RBTREE_INCLUDED
#define RBTREE_INCLUDED

#define T RBTree_T

typedef struct T *T;

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
extern T RBTree_copy(const T tree,
	void *(*copy_data)(const void *data, void *cl), void *cl);

extern void RBTree_free(T *treep,
	void (*free_data)(void *data, void *cl), void *cl);

/**
 * @brief Inserts data into the tree
 *
 * @arg treep Pointer to the root of the tree (may be altered)
 * @arg data pointer to the data structure
 * @arg cmp comparison function
 * @arg cl pointer to data which will be passed unchanged to cmp
 *
 * @return 1 if node was inserted 0 if not
 *
 * @throw Mem_Failed
 */
extern int RBTree_insert(T *treep, const void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl), void *cl);

extern const void *RBTree_remove(T *treep, const void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl), void *cl);

extern const void *RBTree_get(const T tree, const void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl), void *cl);

extern void RBTree_traverse(T tree, int (*apply)(const void *data, void *cl),
	void *cl);

#undef T

#endif
