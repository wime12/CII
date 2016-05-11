#include <stdio.h>
#include "btree.h"
#include "mem.h"
#include <time.h>
#include <stdlib.h>

#define NODES 1000
#define RANGE (NODES * 10)

static int pr(void **data, void *cl) {
    printf("%d\n", (int)(*data));
    return 0;
}

static int cmp(void *d1, void *d2, void *cl) {
    return (int)d1 - (int)d2;
}


static void print_node(BTree_T n1, BTree_T n2) {
    if (n1 && n2)
	printf("    %d -> %d\n", (int)BTree_data(n1), (int)BTree_data(n2));
}

static void output_dot(BTree_T tree) {
    BTree_T left, right;

    if (tree) {
	left = BTree_left(tree);
	right = BTree_right(tree);

	print_node(tree, left);
	print_node(tree, right);

	output_dot(left);
	output_dot(right);
    }
}

int main () {
    BTree_T t = NULL;

    srand(time(NULL));

    puts("Fill tree");
    for (int i = 0; i < NODES; i++) {
	BTree_insert(&t, (void *)(rand() % RANGE), cmp, NULL);
    }

    puts("Print tree pre order");
    BTree_traverse(t, pre_order, pr, NULL);

    puts("Print tree in order");
    BTree_traverse(t, in_order, pr, NULL);

    puts("Print tree post order");
    BTree_traverse(t, post_order, pr, NULL);

    puts("Print tree breadth-first");
    BTree_traverse(t, breadth_first, pr, NULL);

    puts("DOT graph");
    puts("digraph G {");
    output_dot(t);
    puts("}");

    puts("Free tree");
    BTree_free(&t);

    printf("tree: %p\n", (void *)t);

    return 0;
}

