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

static T copy(const T tree, NTree_copy_data_fun_T copy_data, void *cl, T prev) {
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

T NTree_copy(const T tree, NTree_copy_data_fun_T copy_data, void *cl) {
    return copy(tree, copy_data, cl, NULL);
}

inline static int prev_is_parent(T tree) {
    return tree && tree->prev && tree->prev->child == tree;
}

inline static int prev_is_sibling(T tree) {
    return tree && tree->prev && tree->prev->sibling == tree;
}

void NTree_free(T *treep, NTree_free_data_fun_T free_data, void *cl) {
    T temp;
    if (prev_is_parent(*treep))
	(*treep)->prev->child = NULL;
    else if (prev_is_sibling(*treep))
	(*treep)->prev->sibling = NULL;

    while (*treep) {
	if ((*treep)->sibling) {
	    temp = (*treep)->sibling;
	    (*treep)->sibling = temp->child;
	    temp->child = *treep;
	    *treep = temp;
	}
	else {
	    temp = (*treep)->child;
	    if (free_data)
		free_data((void *)(*treep)->data, cl);
	    FREE(*treep);
	    *treep = temp;
	}
    }
}

unsigned int NTree_is_empty(const T tree) {
    return !tree;
}

void size_fun(void **data, int *cl) {
    *cl += 1;
}

unsigned int NTree_size(const T tree) {
    unsigned int cl = 0;
    NTree_traverse(tree, (NTree_apply_fun_T)size_fun, &cl);
    return cl;
}

struct occurrances_cl {
    NTree_compare_fun_T cmp;
    int counter;
    void *cl;
    const void *data;
};

void occurrances_fun(void **datap, struct occurrances_cl *cl) {
    if (cl->cmp(*datap, cl->data, cl->cl) == 0)
	cl->counter += 1;
}

unsigned int NTree_occurrances(const T tree, const void *data,
	NTree_compare_fun_T cmp, void *cl) {
    struct occurrances_cl ocl = {
	.cmp = cmp,
	.counter = 0,
	.cl = cl,
	.data = data,
    };
    NTree_traverse(tree, (NTree_apply_fun_T)occurrances_fun, &ocl);
    return ocl.counter;
}

void NTree_traverse(T tree, NTree_apply_fun_T apply, void *cl) {
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
		apply(&tree->data, cl);
		tree = tree->sibling;
	    }
	}
	else {
	    apply(&tree->data, cl);
	    tree = tree->sibling;
	}
    }
}

// Movement

T NTree_first(const T tree) {
    T t = tree;
    while (prev_is_sibling(t)) {
	t = t->prev;
    }
    return t;
}

T NTree_next(const T tree) {
    return tree ? tree->sibling : NULL;
}

T NTree_previous(const T tree) {
    return tree && prev_is_sibling ? tree->prev : NULL;
}

T NTree_last(const T tree) {
    T t = tree;
    if (t)
        while (t->sibling)
	    t = t->sibling;
    return t;
}

T NTree_find(const T tree, const void *data, NTree_compare_fun_T cmp,
	void *cl) {
    if (!tree || cmp(data, tree->data, cl) == 0)
	return tree;
    else {
	T res = NTree_find(tree->child, data, cmp, cl);
	return res ? res : NTree_find(tree->sibling, data, cmp, cl);
    }
}

T NTree_root(const T tree) {
    T t = tree;
    if (t)
	while (t->prev)
	  t = t->prev;
    return t;
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

void *NTree_set_data(T tree, void *data) {
    assert(tree);
    void *old_data = tree->data;
    tree->data = data;
    return old_data;
}

// Mutation

void NTree_prepend_child(T tree, void *data) {
    assert(tree);
    if (tree->child)
	NTree_insert_before(tree->child, data);
    else {
	tree->child = tree_new(data);
	tree->child->prev = tree;
    }
}

void NTree_append_child(T tree, void *data) {
    assert(tree);
    if (tree->child)
	NTree_insert_after(NTree_last(tree->child), data);
    else {
	tree->child = tree_new(data);
	tree->child->prev = tree;
    }
}

T NTree_insert_before(T tree, void *data) {
    NTree_T new_tree = tree_new(data);
    if (tree) {
	new_tree->sibling = tree;
	if (prev_is_parent(tree)) {
	    new_tree->prev = tree->prev;
	    tree->prev->child = new_tree;
	}
	tree->prev = new_tree; 
    }
    return new_tree;
}

T NTree_insert_after(T tree, void *data) {
    NTree_T new_tree = tree_new(data);
    if (tree) {
	new_tree->sibling = tree->sibling;
	tree->sibling = new_tree;
    }
    return new_tree;
}

T NTree_remove(T tree) {
    if (tree->child)
	return NULL;
    else {
	T next;
	if (prev_is_sibling(tree))
	    tree->prev->sibling = tree->sibling;
        else if (prev_is_parent(tree))
	    tree->prev->child = tree->sibling;
	next = tree->sibling ? tree->sibling : tree->prev;
	FREE(tree);
	return next;
    }
}

#undef T
