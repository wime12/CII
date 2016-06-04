#include "ntree.h"
#include <stdlib.h>
#include <stdio.h>

void pr(int **data, void *cl) {
    printf("%d\n", (int)*data);
}

int main() {
    NTree_T tree = NULL, root;

    tree = NTree_insert_before(tree, (void *)1);
    NTree_insert_after(tree, (void *)2);
    NTree_insert_before(tree, (void *)3);

    NTree_append_child(tree, (void *)4);
    NTree_prepend_child(tree, (void *)5);

    puts("Tree");
    NTree_traverse(tree, (NTree_apply_fun_T)pr, NULL);

    root = NTree_absolute_root(tree);
    puts("Root");
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    puts("Tree after remove");
    NTree_remove(&tree);
    NTree_traverse(tree, (NTree_apply_fun_T)pr, NULL);
    NTree_free(&tree, NULL, NULL);
    puts("Root after delete");
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    NTree_free(&root, NULL, NULL);
    
    return 0;
}
