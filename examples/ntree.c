#include "ntree.h"
#include <stdlib.h>
#include <stdio.h>

void pr(int **data, void *cl) {
    printf("%d\n", **data);
}

int main() {
    NTree_T tree = NULL, root;
    int numbers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    tree = NTree_insert_before(tree, &numbers[2]);
    NTree_insert_after(tree, &numbers[5]);
    NTree_insert_before(tree, &numbers[1]);

    NTree_append_child(tree, &numbers[6]);
    NTree_prepend_child(tree, &numbers[8]);

    puts("Tree");
    NTree_traverse(tree, (NTree_apply_fun_T)pr, NULL);

    root = NTree_absolute_root(tree);
    puts("Root");
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    NTree_free(&tree, NULL, NULL);
    puts("Root after delete");
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    NTree_free(&root, NULL, NULL);
    
    return 0;
}
