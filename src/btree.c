#include "btree.h"
#include "assert.h"
#include "mem.h"
#include <stddef.h>
#include <stdio.h>
#include "seq.h" 

#define T BTree_T

struct BTree_T {
    void *data;
    T parent;
    T left;
    T right;
};

T BTree_new(void *data) {
    T t;
    NEW(t);

    *t = (struct BTree_T){ data };

    return t;
}

T BTree_copy(T tree) {
    if (tree) {
	BTree_T t = BTree_new(tree->data);
	BTree_set_left(t, BTree_copy(BTree_left(tree)));
	BTree_set_right(t, BTree_copy(BTree_right(tree)));
	return t;
    }
    else
	return NULL;
}

void *BTree_data(T tree) {
    assert(tree);
    return tree->data;
}

T BTree_parent(T tree) {
    assert(tree);
    return tree->parent;
}

T BTree_left(T tree) {
    assert(tree);
    return tree->left;
}

T BTree_right(T tree) {
    assert(tree);
    return tree->right;
}

T BTree_set_left(T tree, T left) {
    T prev_left;

    assert(tree);

    prev_left = tree->left;

    if (left) {
	assert(!left->parent);
	left->parent = tree;
    }

    tree->left = left;

    return prev_left;
}

T BTree_set_right(T tree, T right) {
    T prev_right;

    assert(tree);

    prev_right = tree->right;

    if (right) {
	assert(!right->parent);
	right->parent = tree;
    }

    tree->right = right;

    return prev_right;
}

void *BTree_set_data(T tree, void *data) {
    void *prev_data;

    assert(tree);

    prev_data = tree->data;
    tree->data = data;
    return prev_data;
}

void BTree_free(T *treep) {
    assert(treep);

    if (*treep) {
	BTree_free(&(*treep)->left);
	BTree_free(&(*treep)->right);
	FREE(*treep);
    }
}

static void insert(T *treep, void *data,
	int (*cmp)(void *data1, void *data2, void *cl), void *cl) {
    if (*treep == NULL)
	*treep = BTree_new(data);
    else if (cmp(data, (*treep)->data, cl) < 0) 
	insert(&(*treep)->left, data, cmp, cl);
    else if (cmp(data, (*treep)->data, cl) > 0)
	insert(&(*treep)->right, data, cmp, cl);
}

void BTree_insert(T *treep, void *data,
	int (*cmp)(void *data1, void *data2, void *cl), void *cl) {
    assert(treep && cmp);
    insert(treep, data, cmp, cl);
}

static void traverse_in_order(T tree,
	int (*apply)(void **data, void *cl), void *cl) {
    if (tree) {
	traverse_in_order(tree->left, apply, cl);
	if (apply(&tree->data, cl)) return;
	traverse_in_order(tree->right, apply, cl);
    }
}

static void traverse_pre_order(T tree,
	int (*apply)(void **data, void *cl), void *cl) {
    if (tree) {
	if (apply(&tree->data, cl)) return;
	traverse_pre_order(tree->left, apply, cl);
	traverse_pre_order(tree->right, apply, cl);
    }
}

static void traverse_post_order(T tree,
	int (*apply)(void **data, void *cl), void *cl) {
    if (tree) {
	traverse_post_order(tree->left, apply, cl);
	traverse_post_order(tree->right, apply, cl);
	if (apply(&tree->data, cl)) return;
    }
}

static void traverse_breadth_first(T tree,
	int (*apply)(void **data, void *cl), void *cl) {
    if (tree) {
	Seq_T queue = Seq_new(0);

	Seq_addhi(queue, tree);		// enqueue root
	while (Seq_length(queue)) {
	    tree = Seq_remlo(queue);	// dequeue next candidate
	    if (apply(&tree->data, cl)) break;
	    if (tree->left) Seq_addhi(queue, tree->left);	// enqueue left
	    if (tree->right) Seq_addhi(queue, tree->right);	// enqueue right
	}
	Seq_free(&queue);
    }
}

void BTree_traverse(T tree, enum traverse_mode mode,
	int (*apply)(void **data, void *cl), void *cl) {
    assert(apply);
    
    switch (mode) {
	case in_order:
	    traverse_in_order(tree, apply, cl);
	    break;
	case pre_order:
	    traverse_pre_order(tree, apply, cl);
	    break;
	case post_order:
	    traverse_post_order(tree, apply, cl);
	    break;
	case breadth_first:
	    traverse_breadth_first(tree, apply, cl);
	    break;
    }
}
