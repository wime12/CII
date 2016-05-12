#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "mem.h"
#include "atom.h"

#define NCHARS 10
#define NELEMS 1000000

void random_string(char *s, const int len) {
    static const char alphanum[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";
    static const int alphanum_len = sizeof(alphanum) - 1;

    for (int i = 0; i < len; ++i) {
	s[i] = alphanum[rand() % alphanum_len];
    }

    s[len] = 0;
}

int cmp(const char *key1, const char *key2, void *cl) {
    // printf("cmp(\"%s\", \"%s\") = %d\n", key1, key2, strcmp(key1, key2));
    return strcmp(key1, key2);
}

int *copy_data(const int *data) {
    int *new_data;
    NEW(new_data);
    *new_data = *data;
    return new_data;
}

void free_data(int *data) {
    // printf("Free data: %d\n", *data);
    FREE(data);
}

int pr(const void *key, const void *n, void *cl) {
    // printf("map[\"%s\"] = %d\n", (char *)key, *(int *)n);
    return 0;
}

int main(void) {
    char *line = NULL;
    size_t j = 0;

    Map_T map = Map_new(
	(Map_cmp_fun_T)cmp,
	NULL,
	(Map_copy_fun_T)copy_data,
	NULL, (Map_free_fun_T)free_data);

    srand(time(NULL));

    puts("### Fill map");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    char buf[NCHARS];
    int n;

    for (int i = 0; i < NELEMS; i++) {
	n = rand();
	random_string(buf, NCHARS);
	Map_insert(map, Atom_string(buf), copy_data(&n));
    }

    puts("### Traverse map");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    Map_traverse(map, pr, NULL);

    puts("### Copy map -> map2");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    Map_T map2 = Map_copy(map);

    puts("### Free map");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    Map_free(&map);

    puts("### Traverse map2");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    Map_traverse(map2, pr, NULL);

    puts("### Free map2");
    getline(&line, &j, stdin);
    FREE(line); j = 0;

    Map_free(&map2);

    // printf("map = %p, map2 = %p\n", (void *)map, (void *)map2);

    return 0;
}
