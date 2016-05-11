#include <stdio.h>
#include "rbtree.h"
#include "mem.h"
#include <time.h>
#include <stdlib.h>

#define COUNT 20
#define MOD (COUNT * 2)

int cmp(void *d1, void *d2, void *cl) {
    return *(int *)d1 - *(int *)d2;
}

int pr(void *d, void *cl) {
    printf("%d\n", *(int *)d);
    return 0;
}

void free_data(void *d, void *cl) {
    printf("free %d\n", *(int *)d);
    FREE(d);
}

static inline int insert(RBTree_T *t, int *n) {
    return RBTree_insert(t, n, cmp, NULL);
}

int main() {
    RBTree_T tree = NULL;
    int *n;
    int inserted;
    void *d;

    srand(time(NULL));

    for (int i = 1; i <= COUNT; i++) {
	NEW(n); *n = rand() % MOD;
	inserted = insert(&tree, n);
	printf("%d (%d)\n", *n, inserted);
	if (!inserted) FREE(n);
    }

    RBTree_traverse(tree, pr, NULL);


    int r;
    for (int i = 1; i <= COUNT; i++) {
	r = rand() % MOD;
	d = RBTree_remove(&tree, &r, cmp, NULL);
	printf("%d (%p): %d\n", r, d, d ? *(int *)d : -1);
	if (d) FREE(d);
    }

    RBTree_traverse(tree, pr, NULL);
    RBTree_traverse(tree, pr, NULL);
    RBTree_traverse(tree, pr, NULL);

    RBTree_free(&tree, free_data, NULL);

    printf("tree = %p\n", (void *)tree);

    return 0;
}
