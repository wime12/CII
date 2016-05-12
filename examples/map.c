#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "mem.h"
#include "atom.h"

int cmp(const char *key1, const char *key2, void *cl) {
    printf("cmp(\"%s\", \"%s\") = %d\n", key1, key2, strcmp(key1, key2));
    return strcmp(key1, key2);
}

int *copy_data(const int *data) {
    int *new_data;
    NEW(new_data);
    *new_data = *data;
    return new_data;
}

void free_data(int *data) {
    printf("Free data: %d\n", *data);
    FREE(data);
}

int pr(const char *key, const int *n, void *cl) {
    printf("map[%s] = %d\n", key, *n);
    return 0;
}

int main(void) {
    Map_T map = Map_new(
	(Map_cmp_fun_T)cmp,
	NULL,
	(Map_copy_fun_T)copy_data,
	NULL, (Map_free_fun_T)free_data);

    int *n[6];
    srand(time(NULL));
    for (int i = 0; i < 6; i++) {
	NEW(n[i]);
	*n[i] = rand() % 20;
    }

    Map_insert(map, Atom_string("zbc"), n[0]);
    Map_insert(map, Atom_string("def"), n[1]);
    Map_insert(map, Atom_string("lkjas"), n[2]);
    Map_insert(map, Atom_string("98zdfg"), n[3]);
    Map_insert(map, Atom_string("ei"), n[4]);
    Map_insert(map, Atom_string("pouaer"), n[5]);

    Map_traverse(map, (Map_apply_fun_T)pr, NULL);

    Map_T map2 = Map_copy(map);

    Map_free(&map);

    puts("### Print copy");
    Map_traverse(map2, (Map_apply_fun_T)pr, NULL);

    Map_free(&map2);

    return 0;
}
