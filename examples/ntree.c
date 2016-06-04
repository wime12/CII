#include "ntree.h"
#include <stdlib.h>
#include <stdio.h>

void pr(int **data, void *cl) {
    printf("%d ", (int)*data);
}

int main() {
    NTree_T tree = NULL, root;

    tree = NTree_insert_before(tree, (void *)1);
    NTree_insert_after(tree, (void *)2);
    printf("Insert_after: %d\n", (int)NTree_data(tree));
    NTree_insert_before(tree, (void *)3);
    printf("Insert_before: %d\n", (int)NTree_data(tree));

    NTree_append_child(tree, (void *)4);
    printf("Append_child: %d\n", (int)NTree_data(tree));
    NTree_prepend_child(tree, (void *)5);
    printf("Prepend_child: %d\n", (int)NTree_data(tree));

    printf("Previous: %d\n", (int)NTree_data(NTree_previous(tree)));

    puts("Tree");
    NTree_traverse(tree, (NTree_apply_fun_T)pr, NULL);
    puts("");
    printf("Traverse: %d\n", (int)NTree_data(tree));

    root = NTree_absolute_root(tree);
    printf("Root: %d\n", (int)NTree_data(root));
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    puts("");

    // Remove unremovable root
    NTree_remove(&tree);
    printf("Tree root after remove: %d\n", (int)NTree_data(tree));
    puts("Tree after remove");
    NTree_traverse(tree, (NTree_apply_fun_T)pr, NULL);
    puts("");

    // Remove removable root
    tree = NTree_first_child(tree);
    NTree_remove(&tree);
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    puts("");

    NTree_free(&tree, NULL, NULL);
    puts("Root after delete");
    NTree_traverse(root, (NTree_apply_fun_T)pr, NULL);
    puts("");
    NTree_free(&root, NULL, NULL);
    
    return 0;
}
