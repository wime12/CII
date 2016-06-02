#include "ntree.h"
#include <stdlib.h>
#include <stdio.h>

void pr(int *data, void *cl) {
    printf("%d\n", *data);
}

int main() {
    NTCursor_T c = NTCursor_new(NULL);
    int numbers[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    NTCursor_insert_before(c, &numbers[2]);
    NTCursor_insert_after(c, &numbers[5]);
    NTCursor_insert_before(c, &numbers[1]);

    printf("%d\n", *(int *)NTCursor_get(c));

    NTCursor_free(&c);
    
    return 0;
}
