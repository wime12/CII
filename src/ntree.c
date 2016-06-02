#include <stdlib.h>
#include <ntree.h>
#include "mem.h"
#include "assert.h"

#define T NTree_T

struct T {
    void *data;
    T prev, child, sibling;
};

inline static T tree_new(void *data) {
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
	if (tree->sibling == NULL && tree->child == NULL) {
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

int size_fun(void **data, int *cl) {
    *cl += 1;
    return 0;
}

unsigned int NTree_size(const T tree) {
    unsigned int cl = 0;
    NTree_traverse(tree, (int (*)(void **, void *))size_fun, &cl);
    return cl;
}

struct occurrances_cl {
    int (*cmp)(const void *data1, const void *data2, void *cl);
    int counter;
    void *cl;
    void *data;
};

int occurrances_fun(void **datap, struct occurrances_cl *cl) {
    if (cl->cmp(*datap, cl->data, cl->cl) == 0)
	cl->counter += 1;
    return 0;
}

unsigned int NTree_occurrances(const T tree, const void *data,
	int (*cmp)(const void *data1, const void *data2, void *cl),
	void *cl) {
    struct occurrances_cl ocl = {
	.cmp = cmp,
	.counter = 0,
	.cl = cl,
	.data = data,
    };
    NTree_traverse(tree, (int (*)(void **, void *))occurrances_fun, &ocl);
    return ocl.counter;
}

int occurs_fun(void **datap, struct occurrances_cl *cl) {
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
    NTree_traverse(tree, (int (*)(void **, void *))occurs_fun, &ocl);
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

inline static int prev_is_parent(T tree) {
    return tree && tree->prev->child == tree;
}

inline static int prev_is_sibling(T tree) {
    return tree && tree->prev->sibling == tree;
}

inline static int is_root(T tree) {
    return tree && tree->prev == NULL;
}

// Movement

T NTree_first(const T tree) {
    while (prev_is_sibling(tree)) {
	tree = tree->prev;
    }
    return tree;
}

T NTree_next(const T tree) {
    return tree ? tree->sibling : NULL;
}

T NTree_previous(const T tree) {
    return tree && prev_is_sibling ? tree->prev : NULL;
}

void NTree_last(const T tree) {
    if (tree)
        while (tree->sibling)
	    tree = tree->sibling;
    return tree;
}

T NTree_find(const T tree, const void *data,
        int (*cmp)(const void *data1, const void *data2, void *cl),
	void *cl) {
    if (tree) {
	if (!tree || (cmp(data, tree->data, cl) == 0))
	    return tree;
	else
	    return find_entry(tree->child, data, cmp, cl)
		   || find_entry(tree->sibling, data, cmp, cl);
}

T NTree_root(const T tree) {
    if (tree)
	while (tree->prev)
	  tree = tree->prev;
    return tree;
}

T NTree_first_child(const T tree) {
    return tree ? tree->child : NULL;
}

// Tests

int NTree_is_first_sibling(const T tree) {
    return tree && prev_is_parent(tree);
}

int NTree_is_last_sibling(const T tree) {
    return tree && !tree->sibling;
}

int NTree_is_root(const T tree) {
    return tree && !tree->prev;
}

int NTree_has_children(const T tree) {
    return tree && tree->child;
}

// Data

void *NTree_data(const T tree) {
    assert(tree);
    return tree->data;
}

void *NTree_set_data(T tree, const void *data) {
    assert(tree);
    void *old_data = tree->data;
    tree->data = data;
    return old_data;
}

// Mutation

void NTree_prepend_child(T tree, const void *data) {
    assert(tree);
    if (tree->child)
	NTree_insert_before(tree->child, data);
    else
	tree->child = tree_new(data);
}

void NTree_append_child(T tree, const void *data) {
    assert(tree);
    if (tree->child)
	NTree_insert_after(NTree_last(tree->child), data);
    else
	tree->child = tree_new(data);
}

// *** CONTINUE HERE ***

void NTCursor_insert_before(T cursor, void *data) {
    assert(cursor);
    NTree_T new_tree = tree_new(data);
    if (cursor->tree) {
	new_tree->sibling = cursor->tree;
	if (prev_is_parent(cursor->tree)) {
	    new_tree->prev = cursor->tree->prev;
	    cursor->tree->prev->child = new_tree;
	}
	cursor->tree->prev = new_tree; 
    }
    else 
	cursor->tree = new_tree;
}

void NTCursor_insert_after(T cursor, void *data) {
    assert(cursor);
    NTree_T new_tree = tree_new(data);
    if (cursor->tree) {
	new_tree->sibling = cursor->tree->sibling;
	cursor->tree->sibling = new_tree;
    }
    else
	cursor->tree = new_tree;
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
