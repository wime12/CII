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
extern T RBTree_new(void *x);

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
extern T RBTree_copy(T tree,
	void *(*copy_data)(void *data_data, void *cl), void *cl);

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
extern int RBTree_insert(T *treep, void *data,
    int (*cmp)(void *data1, void *data2, void *cl), void *cl);

extern void *RBTree_remove(T *treep, void *data,
    int (*cmp)(void *data1, void *data2, void *cl), void *cl);

extern void *RBTree_get(T tree, void *data,
    int (*cmp)(void *data1, void *data2, void *cl), void *cl);

extern void RBTree_traverse(T tree, int (*apply)(void *data, void *cl),
	void *cl);

#undef T

#endif
