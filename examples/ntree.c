#include "ntree.h"
#include <stdlib.h>
#include <stdio.h>

void pr(int **data, void *cl) {
    printf("%d\n", **data);
}

int main() {
    NTree_T tree = NULL;
    int numbers[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    tree = NTree_insert_before(tree, &numbers[2]);
    NTree_insert_after(tree, &numbers[5]);
    NTree_insert_before(tree, &numbers[1]);

    NTree_append_child(tree, &numbers[6]);
    NTree_prepend_child(tree, &numbers[8]);

    NTree_traverse(tree, (void (*)(void **, void *))pr, NULL);

    tree = NTree_root(tree);
    NTree_free(&tree, NULL, NULL);
    
    return 0;
}
