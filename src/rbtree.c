#include "rbtree.h"
#include "assert.h"
#include "mem.h"
#include <stddef.h>
#include <stdio.h>

#define T RBTree_T

enum Color { black = 0, red = 1 };

enum Direction { left = 0, right = 1 };

struct RBTree_T {
    const void *data;
    unsigned int color : 1;
    T children[2];
};

T RBTree_new(const void *data) {
    T t;
    NEW(t);

    *t = (struct RBTree_T){ .data = data, .color = red };

    return t;
}

void RBTree_free(T *tree, RBTree_free_data_fun_T free_data, void *cl) {
    assert(tree);
    T temp;
    while (*tree) {
	if ((*tree)->children[left]) {
	    temp = (*tree)->children[left];
	    (*tree)->children[left] = temp->children[right];
	    temp->children[right] = *tree;
	    *tree = temp;
	}
	else {
	    temp = (*tree)->children[right];
	    if (free_data) free_data((void *)(*tree)->data, cl);
	    FREE(*tree);
	    *tree = temp;
	}
    }
}

inline static void set_color(T tree, enum Color color) {
    tree->color = color;
}

inline static enum Color color(T tree) {
    return tree ? tree->color : black;
}

T RBTree_copy(T tree, RBTree_copy_data_fun_T copy_data, void *cl) {
    if (tree) {
	T t = RBTree_new(copy_data ? copy_data(tree->data, cl) : tree->data);
	set_color(t, color(tree));
	t->children[left] = RBTree_copy(tree->children[left], copy_data, cl);
	t->children[right] = RBTree_copy(tree->children[right], copy_data, cl);
	return t;
    }
    else
	return NULL;
}

inline static T rotate_single(T tree, enum Direction dir) {
    T save = tree->children[!dir];

    tree->children[!dir] = save->children[dir];
    save->children[dir] = tree;

    set_color(tree, red);
    set_color(save, black);

    return save;
}

inline static T rotate_double(T tree, enum Direction dir) {
    tree->children[!dir] = rotate_single(tree->children[!dir], !dir);
    return rotate_single(tree, dir);
}

/**
 * Returns 1 if data was inserted and 0 if not.
 */
static int insert(T *tree, const void *data, RBTree_compare_fun_T cmp,
	void *cl) {
    int inserted = 0;
    if (*tree == NULL) {
	*tree = RBTree_new(data);
	inserted = 1;
    }
    else {
	struct RBTree_T head = { 0 }; // False head
	T g = NULL, t = &head, p = NULL, q;
	q = t->children[right] = *tree;
	int dir = left, last_dir = dir;

	for (;;) {

	    if (q == NULL) {
		p->children[dir] = q = RBTree_new(data);
		inserted = 1;
	    }
	    else if (color(q->children[left]) == red
			&& color(q->children[right]) == red) {
		set_color(q, red);
		set_color(q->children[left], black);
		set_color(q->children[right], black);
	    }

	    if (color(q) == red && color(p) == red) {
		int dir2 = (t->children[right] == g);

		if (q == p->children[last_dir])
		    t->children[dir2] = rotate_single(g, !last_dir);
		else
		    t->children[dir2] = rotate_double(g, !last_dir);
	    }

	    if (cmp(q->data, data, cl) == 0) {
		break;
	    }

	    last_dir = dir;
	    dir = cmp(q->data, data, cl) < 0;

	    if (g) t = g;

	    g = p, p = q;
	    q = q->children[dir];
	}

	(*tree) = head.children[right];
    }
    set_color((*tree), black);

    return inserted;
}

int RBTree_insert(T *tree, const void *data, RBTree_compare_fun_T cmp,
	void *cl) {
    assert(tree);
    return insert(tree, data, cmp, cl);
}

static const void *rb_remove(T *tree, const void *data,
	RBTree_compare_fun_T cmp, void *cl) {
    const void *fd = NULL;

    if (*tree) {
	struct RBTree_T head = { 0 };
	T q = &head, p = NULL, g = NULL, f = NULL;
	enum Direction dir = right;

	q->children[right] = (*tree);

	while (q->children[dir]) {
	    int last_dir = dir;

	    g = p, p = q;
	    q = q->children[dir];
	    dir = cmp(q->data, data, cl) < 0;

	    if (cmp(q->data, data, cl) == 0) {
		f = q;
		fd = q->data;
	    }

	    if (color(q) == black && color(q->children[dir]) == black) {
		if (color(q->children[!dir]) == red)
		    p = p->children[last_dir] = rotate_single(q, dir);
		else { // if (color(q->children[!dir]) == black)
		    T s = p->children[!last_dir];

		    if (s) {
			if (color(s->children[left]) == black
				&& color(s->children[right]) == black) {
			    set_color(p, black);
			    set_color(s, red);
			    set_color(q, red);
			}
			else {
			    int dir2 = g->children[right] == p;

			    if (color(s->children[last_dir]) == red)
				g->children[dir2] = rotate_double(p, last_dir);
			    else if (color(s->children[!last_dir]) == red)
				g->children[dir2] = rotate_single(p, last_dir);

			    set_color(q, red);
			    set_color(g->children[dir2], red);
			    set_color(g->children[dir2]->children[left], black);
			    set_color(g->children[dir2]->children[right],
					black);
			}
		    }
		}
	    }
	}

	if (f) {
	    f->data = q->data;
	    p->children[p->children[right] == q] =
		    q->children[q->children[left] == NULL];
	    FREE(q);
	}

	(*tree) = head.children[right];

	if (*tree) set_color(*tree, black);
    }
    return fd;
}

const void *RBTree_remove(T *tree, const void *data, RBTree_compare_fun_T cmp,
	void *cl) {
    assert(tree && cmp);

    return rb_remove(tree, data, cmp, cl);
}

const void *RBTree_get(T tree, const void *data, RBTree_compare_fun_T cmp,
	void *cl) {
    int c;
    while(tree && (c = cmp(tree->data, data, cl))) {
	if (c > 0)
	    tree = tree->children[left];
	else
	    tree = tree->children[right];
    }
    return tree;
}

void RBTree_traverse(T tree, RBTree_apply_fun_T apply, void *cl) {
    T temp;
    while (tree) {
	if ((temp = tree->children[left])) {
	    while (temp->children[right] && temp->children[right] != tree)
		temp = temp->children[right];
	    if (temp->children[right] == NULL) {
		temp->children[right] = tree;
		tree = tree->children[left];
	    }
	    else {
		temp->children[right] = NULL;
		apply(tree->data, cl);
		tree = tree->children[right];
	    }
	}
	else {
	    apply(tree->data, cl);
	    tree = tree->children[right];
	}
    }
}

unsigned int RBTree_depth(const T tree) {
    if (tree) {
	unsigned int left_height = RBTree_depth(tree->children[left]);
	unsigned int right_height = RBTree_depth(tree->children[right]);
	return (left_height > right_height ? left_height : right_height) + 1;
    }
    else
	return 0;
}

static void inc(const void *x, unsigned int *n) {
    *n += 1;
}

unsigned int RBTree_size(const T tree) {
    unsigned int n = 0;
    RBTree_traverse(tree, (RBTree_apply_fun_T)inc, &n);
    return n;
}

