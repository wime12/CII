#ifndef NTREE_INCLUDED
#define NTREE_INCLUDED

#define T NTree_T

typedef struct T *T;

T NTree_new();

T NTree_copy(const T tree);

void NTree_free(T *treep);

size_t NTree_size(const T tree);

int NTree_empty(const T tree);

void *NTree_get(const T tree, void *obj);

unsigned int NTree_count(const T tree, void *obj);

void NTree_each(const T tree, int (*apply)(void *obj, void *cl), void *cl);

#undef T

#define T NTreeIterator_T

typedef struct T *T;

T NTreeIterator_new(T tree);

/* child -> first child */
void NTreeIterator_first(T iterator);

/* child -> next child */
void NTreeIterator_next(T iterator);

/* child -> previous child */
void NTreeIterator_prev(T iterator);

/* child -> last child */
void NTreeIterator_last(T iterator);

/* -> obj (searched from root) */
void NTreeIterator_move(T iterator, void *obj);

/* -> root */
void NTreeIterator_root(T iterator);

/* child -> parent */
void NTreeIterator_parent(T iterator);

/* parent -> first child */
void NTreeIterator_child(T iterator);

/* first child? */
int NTreeIterator_isFirst(T iterator);

/* last child? */
int NTreeIterator_isLast(T iterator);

/* root? */
int NTreeIterator_isRoot(T iterator);

/* does the current node have children */
int NTreeIterator_hasChildren(T iterator);

/* set the object of the current node, return the old object */
void *NTreeIterator_set(T iterator, void *obj);

/* prepend object to the list of children of the current node (= parent) */
void NTreeIterator_prepend(T iterator, void *obj);

/* append object to the list of children of the current node (= parent) */
void NTreeIterator_append(T iterator, void *obj);

/* insert a new child before this node */
void NTreeIterator_before(T iterator, void *obj);

/* insert a new child after this node */
void NTreeIterator_after(T iterator, void *obj);

