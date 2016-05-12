#include <stdio.h>
#include <string.h>
#include "map.h"
#include "mem.h"

int cmp(const char *key1, const char *key2, void *cl) {
    printf("cmp(\"%s\", \"%s\") = %d\n", key1, key2, strcmp(key1, key2));
    return strcmp(key1, key2);
}

char *copy_key(const char *key) {
    size_t s = strlen(key);
    char *new_key = CALLOC(s + 1, sizeof(char));
    strcpy(new_key, key);
    return new_key;
}

int *copy_data(const int *data) {
    int *new_data;
    NEW(new_data);
    *new_data = *data;
    return new_data;
}

void free_key(char *key) {
    FREE(key);
}

void free_data(int *data) {
    FREE(data);
}

int pr(const char *key, const int *n, void *cl) {
    printf("map[%s] = %d\n", key, *n);
    return 0;
}

int main(void) {
    Map_T map = Map_new(
	(Map_cmp_fun_T)cmp,
	(Map_copy_fun_T)copy_key,
	(Map_copy_fun_T)copy_data,
	NULL, (Map_free_fun_T)free_data);

    int *n[6];
    for (int i = 0; i < 6; i++) {
	NEW(n[i]);
	*n[i] = i;
    }

    Map_insert(map, "zbc", n[0]);
    Map_insert(map, "def", n[1]);
    Map_insert(map, "lkjas", n[2]);
    Map_insert(map, "98zdfg", n[3]);
    Map_insert(map, "ei", n[4]);
    Map_insert(map, "pouaer", n[5]);

    Map_traverse(map, (Map_apply_fun_T)pr, NULL);

    Map_T map2 = Map_copy(map);

    Map_free(&map);

    puts("### Print copy");
    Map_traverse(map2, (Map_apply_fun_T)pr, NULL);

    Map_free(&map2);

    return 0;
}
