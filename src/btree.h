#ifndef TREE_INCLUDED
#define TREE_INCLUDED

#define T BTree_T

typedef struct T *T;

enum traverse_mode {
    in_order,
    pre_order,
    post_order,
    breadth_first
};

/**
 * @brief Makes a new node initialized with data.
 *
 * A new node is created, which contains data `x`.
 *
 * @param x The data content of the node
 *
 * @return a new node
 */
extern T BTree_new(void *x);

/**
 * @brief Copies the node and its children
 *
 * A shallow copy of the node and its children is created. The data is
 * not copied.
 *
 * @param The root of the tree
 *
 * @return A new tree
 */
extern T BTree_copy(T tree);

/**
 * @brief Returns the data of the node
 *
 * @param The node
 *
 * @return The data of the node
 */
extern void *BTree_data(T tree);

/**
 * @brief Returns the parent of node.
 *
 * @param The child of the parent
 *
 * @return The parent of the child node.
 */
extern T BTree_parent(T tree);

/** 
 * @brief Returns the left branch of the current node.
 *
 * @param node The node
 *
 * @return The left branch.
 */
extern T BTree_left(T tree);

/** 
 * @brief Returns the right branch of the current node.
 *
 * @param node The node
 *
 * @return The branch branch.
 */
extern T BTree_right(T tree);

/**
 * @brief Set the left branch.
 *
 * Sets the left branch of the tree.
 * `left` may also be NULL. This results in cutting
 * the left branch off the tree.
 *
 * @param node The node
 * @param left The left branch to add
 *
 * @return The previous left branch
 */
extern T BTree_set_left(T tree, T left);

/**
 * @brief Set the right branch.
 *
 * Sets the right branch of the tree.
 * `right` may also be NULL. This results in cutting
 * the right branch off the tree.
 *
 * @param node The node
 * @param right The right branch to add
 *
 * @return The previous left branch
 */
extern T BTree_set_right(T tree, T right);

/**
 * @brief Free the tree.
 *
 * Frees the tree -- the parent and all of its children.
 * The data is not freed.
 *
 * @param tree The tree
 */

extern void *BTree_set_data(T tree, void *data);

extern void BTree_free(T *treep);

extern void BTree_insert(T *treep,
    void *data, int (*cmp)(void *data1, void *data2, void *cl), void *cl);

extern void BTree_traverse(T tree, enum traverse_mode,
    int (*apply)(void **data, void *cl), void *cl);

#undef T

#endif
