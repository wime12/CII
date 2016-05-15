#ifndef NTREE_INCLUDED
#define NTREE_INCLUDED

#define T NTree_T

typedef struct T *T;

extern T NTree_new(void *data);

extern T NTree_copy(const T tree,
    void *(*copy_data)(const void *data, void *cl), void *cl);

extern void NTree_free(T *treep,
    void *(free_data)(void *data, void *cl), void *cl);

extern unsigned int NTree_is_empty(const T tree);

extern unsigned int NTree_size(const T tree);

extern int NTree_occurs(const T tree, void *data,
    int (*comp)(const void *data1, const void *data2, void *cl), void *cl);

extern unsigned int NTree_occurrances(const T tree, void *data,
    int (*comp)(const void *data1, const void *data2, void *cl), void *cl);

extern void NTree_traverse(T tree,
    int (*apply)(void **datap, void *cl), void *cl);

#undef T

#define T NTCursor_T

typedef struct T *T;

extern T NTCursor_new(NTree_T tree);

// Movement

/**
 * Move to the first child (in the list of children)
 */
extern T NTCursor_first(T cursor);

/**
 * Move to the next child (in the list of children)
 * Returns NULL if there is no next child.
 */
extern T NTCursor_next(T cursor);

/**
 * Move to the previous child (in the list of children)
 * Returns NULL if there is no previous child.
 */
extern T NTCursor_previous(T cursor);

/**
 * Move to the last child (in the list of children)
 * Returns NULL if there is no last child.
 */
extern T NTCursor_last(T cursor);

/**
 * Move the cursor to the position of the next occurrance of
 * data. The equality is determined by the cmp function which
 * returns 0 for equality and a value != 0 otherwise. The last
 * argument is data passed unchanged to the cmp function.
 * Returns NULL if the position could not be found.
 */
extern T NTCursor_move_to(T cursor, void *data,
    int (*cmp)(const void *data1, const void *data2, void *cl),
    void *cl);

/**
 * Move to the root of the tree.
 * Returns NULL if the tree is empty.
 */
extern T NTCursor_root(T cursor);

/**
 * Move to the first child node from the parent.
 * Returns NULL if there is none.
 */
extern T NTCursor_first_child(T cursor);

// Tests

/**
 * Returns 1 if the cursor is on a first child, 0 otherwise.
 */
extern int NTCursor_on_first(T cursor);

/**
 * Returns 1 if the cursor is on a last child, 0 otherwise.
 */
extern int NTCursor_on_last(T cursor);

/**
 * Returns 1 if the cursor is on the root of the tree, 0 otherwise.
 */
extern int NTCursor_on_root(T cursor);

/**
 * Returns 1 if this parent has children
 */
extern int NTCursor_has_children(T cursor);

// Data

/**
 * Get the data on the current position.
 */
extern void *NTCursor_get(T cursor);

/**
 * Set the data at the current position and return the old data.
 */
extern void *NTCursor_set(T cursor, void *data);

// Mutation

/**
 * Prepend a child to the list of children of the current position.
 * The cursor stays at the current position.
 */
extern void NTCursor_prepend_child(T cursor, void *data);

/**
 * Append a child to the list of children of the current position.
 * The cursor stays at the current position.
 */
extern void NTCursor_append_child(T cursor, void *data);

/**
 * Insert a sibling before this position.
 * The cursor stays at the current position.
 */
extern void NTCursor_insert_before(T cursor, void *data);

/**
 * Insert a sibling after this position.
 * The cursor stazs at the current position.
 */
extern void NTCursor_insert_after(T cursor, void *data);

/**
 * Remove the current node if it has no children.
 * The data of the current node is returned if the node
 * was removed or NULL if it could not be removed.
 * The cursor moves to the next sibling if there is one or
 * to the previous sibling if there is no next sibling but a
 * previous one or to the parent if there is neither a
 * next nor a previous sibling.
 */
extern void *NTCursor_remove(T cursor);

#undef T

#endif
