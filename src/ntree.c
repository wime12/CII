#include <stdlib.h>
#include <ntree.h>
#include "mem.h"
#include "assert.h"

#define T NTree_T

struct NTree_T {
    void *data;
    T prev, child, sibling;
};

T NTree_new(void *data) {
    T tree;
    NEW(tree);
    *tree = (struct NTree_T) {
	.data = data,
	.prev = NULL,
	.child = NULL,
	.sibling = NULL,
    };
    return tree;
}

static T copy(const T tree, void *(*copy_data)(const void *data, void *cl),
	void *cl, T prev) {
    if (tree) {
	T new_tree;
	NEW(new_tree);
	*new_tree = (struct NTree_T) {
	    .data = copy_data ? copy_data(tree->data, cl) : tree->data,
	    .prev = prev,
	    .child = copy(tree->child, copy_data, cl, new_tree),
	    .sibling = copy(tree->sibling, copy_data, cl, new_tree)
	};
	return new_tree;
    }
    else
	return NULL;
}

T NTree_copy(const T tree,
	void *(*copy_data)(const void *data, void *cl), void *cl) {
    return copy(tree, copy_data, cl, NULL);
}

void NTree_free(T *treep,
    	void *(free_data)(void *data, void *cl), void *cl) {
    T tree = *treep, prev;
    while (tree) {
	if (tree->sibling == NULL & tree->child == NULL) {
	    prev = tree->prev;
	    FREE(tree);
	    tree = prev;
	}
	else if (tree->child) 
	    tree = tree->child;
	else if (tree->sibling) 
	    tree = tree->sibling;
    }
    *treep = NULL;
}

unsigned int NTree_is_empty(const T tree) {
    return !tree;
}

int size(void **data, int *cl) {
    *cl += 1;
    return 0;
}

unsigned int NTree_size(const T tree) {
    unsigned int cl = 0;
    NTree_traverse(tree, (int (*)(void **, void *))size, &cl);
    return cl;
}

struct occurrances_cl {
    int (*cmp)(const void *data1, const void *data2, void *cl);
    int counter;
    void *cl;
    void *data;
};

int occurrances(void **datap, struct occurrances_cl *cl) {
    if (cl->cmp(*datap, cl->data, cl->cl) == 0)
	cl->counter += 1;
    return 0;
}

unsigned int NTree_occurrances(const T tree, void *data,
	int (*cmp)(const void *data1, const void *data2, void *cl),
	void *cl) {
    struct occurrances_cl ocl = {
	.cmp = cmp,
	.counter = 0,
	.cl = cl,
	.data = data,
    };
    NTree_traverse(tree, (int (*)(void **, void *))occurrances, &ocl);
    return ocl.counter;
}

int occurs(void **datap, struct occurrances_cl *cl) {
    if (cl->cmp(*datap, cl->data, cl->cl) == 0) {
	cl->counter = 1;
	return 1;
    }
    return 0;
}

int NTree_occurs(const T tree, void *data,
	int (*cmp)(const void *data1, const void *data2, void *cl),
	void *cl) {
    struct occurrances_cl ocl = {
	.cmp = cmp,
	.counter = 0,
	.cl = cl,
	.data =data
    };
    NTree_traverse(tree, (int (*)(void **, void *))occurs, &ocl);
    return ocl.counter;
}

void NTree_traverse(T tree,
	int (*apply)(void **datap, void *cl), void *cl) {
    T temp;
    while (tree) {
	if ((temp = tree->child)) {
	    while (temp->sibling && temp->sibling != tree)
		temp = temp->sibling;
	    if (temp->sibling == NULL) {
		temp->sibling = tree;
		tree = tree->child;
	    }
	    else {
		temp->sibling = NULL;
		if (apply(&tree->data, cl)) break;
		tree = tree->sibling;
	    }
	}
	else {
	    if (apply(&tree->data, cl)) break;
	    tree = tree->sibling;
	}
    }
}

#undef T

#define T NTCursor_T

struct T {
    NTree_T tree;
};

T NTCursor_new(NTree_T tree) {
    assert(tree);
    T cursor;
    NEW(cursor);
    cursor->tree = tree;
    return cursor;
}

void NTCursor_free(NTCursor_T *cursorp) {
    FREE(*cursorp);
}

inline static int prev_is_parent(NTree_T tree) {
    return tree && tree->prev->child == tree;
}

inline static int prev_is_sibling(NTree_T tree) {
    return tree && tree->prev->sibling == tree;
}

inline static int is_root(NTree_T tree) {
    return tree && tree->prev == NULL;
}

// Movement

void NTCursor_first(T cursor) {
    assert(cursor && cursor->tree);
    while (prev_is_sibling(cursor->tree)) {
	cursor->tree = cursor->tree->prev;
    }
}

int NTCursor_next(T cursor) {
    assert(cursor && cursor->tree);
    if (cursor->tree->child) {
	cursor->tree = cursor->tree->child;
	return 1;
    }
    else
	return 0;
}

int NTCursor_previous(T cursor) {
    assert(cursor && cursor->tree);
    if (prev_is_sibling(cursor->tree)) {
	cursor->tree = cursor->tree->prev;
	return 1;
    }
    else
	return 0;
}

void NTCursor_last(T cursor) {
    assert(cursor && cursor->tree);
    while (cursor->tree->child) {
	cursor->tree = cursor->tree->child;
    }
}

static int find_entry(NTree_T tree, void *data,
        int (*cmp)(const void *data1, const void *data2, void *cl),
	void *cl) {
    return tree
	   && ((cmp(data, tree->data, cl) == 0)
	       || find_entry(tree->child, data, cmp, cl)
	       || find_entry(tree->sibling, data, cmp, cl));
}

int NTCursor_find(T cursor, void *data,
   	int (*cmp)(const void *data1, const void *data2, void *cl),
        void *cl) {
    assert(cursor && cursor->tree);
    return find_entry(cursor->tree, data, cmp, cl);
}

void NTCursor_root(T cursor) {
    assert(cursor && cursor->tree);
    while (cursor->tree->prev)
	cursor->tree = cursor->tree->prev;
}

int NTCursor_first_child(T cursor) {
    assert(cursor && cursor->tree);
    if (cursor->tree->child) {
	cursor->tree = cursor->tree->child;
	return 1;
    }
    else
	return 0;
}

// Tests

int NTCursor_on_first(T cursor) {
    assert(cursor && cursor->tree);
    return prev_is_parent(cursor->tree);
}

int NTCursor_on_last(T cursor) {
    assert(cursor && cursor->tree);
    return cursor->tree->child == NULL;
}

int NTCursor_on_root(T cursor) {
    assert(cursor);
    return is_root(cursor->tree);
}

int NTCursor_has_children(T cursor) {
    assert(cursor && cursor->tree);
    return cursor->tree->child != NULL;
}

// Data

void *NTCursor_get(T cursor) {
    assert(cursor && cursor->tree);
    return cursor->tree->data;
}

void *NTCursor_set(T cursor, void *data) {
    assert(cursor && cursor->tree);
    void *old_data = cursor->tree->data;
    cursor->tree->data = data;
    return old_data;
}

// Mutation

void NTCursor_prepend_child(T cursor, void *data) {
    assert(cursor && cursor->tree);
    NTree_T new_tree = NTree_new(data);
    new_tree->child = cursor->tree->child;
    new_tree->prev = cursor->tree;
    new_tree->child->prev = new_tree;
    cursor->tree->child = new_tree;
}

/**
 * Append a child to the list of children at the current position.
 * The cursor stays at the current position.
 */
void NTCursor_append_child(T cursor, void *data) {
    T new_cursor = NTCursor_new(cursor->tree);
    NTCursor_first_child(new_cursor);
    NTCursor_last(new_cursor);
    NTCursor_insert_after(new_cursor, data);
    NTCursor_free(&new_cursor);
}

void NTCursor_insert_before(T cursor, void *data) {
    assert(cursor && cursor->tree);
    NTree_T new_tree = NTree_new(data);
    new_tree->sibling = cursor->tree;
    if (prev_is_parent(cursor->tree)) {
	new_tree->prev = cursor->tree->prev;
	cursor->tree->prev->child = new_tree;
    }
    cursor->tree->prev = new_tree; 
}

void NTCursor_insert_after(T cursor, void *data) {
    assert(cursor && cursor->tree);
    NTree_T new_tree = NTree_new(data);
    new_tree->sibling = cursor->tree->sibling;
    cursor->tree->sibling = new_tree;
}

void *NTCursor_remove(T cursor) {
    assert(cursor && cursor->tree);
    NTree_T tree = cursor->tree;
    if (tree->child)
	return NULL;
    else {
	void *data = tree->data;
	if (prev_is_sibling(tree))
	    tree->prev->sibling = tree->sibling;
        else if (prev_is_parent(tree))
	    tree->prev->child = tree->sibling;
	cursor->tree = tree->sibling ? tree->sibling : tree->prev;
	FREE(tree);
	return data;
    }
}

#undef T
