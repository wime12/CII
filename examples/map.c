#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "mem.h"
#include "atom.h"

#define NCHARS 4
#define NELEMS 100000

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

void wait_return(const char *message) {
    char *line = NULL;
    size_t j = 0;
    puts(message);
    getline(&line, &j, stdin);
    FREE(line);
}

int main(void) {
    Map_T map = Map_new(
	(Map_cmp_fun_T)cmp,
	NULL,
	(Map_copy_fun_T)copy_data,
	NULL, (Map_free_fun_T)free_data);

    srand(time(NULL));

    wait_return("### Fill map");
    printf("map->length = %d\n", Map_length(map));
    char buf[NCHARS];
    int n;

    for (int i = 0; i < NELEMS; i++) {
	n = rand();
	random_string(buf, NCHARS);
	Map_insert(map, Atom_string(buf), copy_data(&n));
    }
    printf("map->length = %d\n", Map_length(map));

    wait_return("### Traverse map");
    Map_traverse(map, pr, NULL);

    wait_return("### Copy map -> map2");
    Map_T map2 = Map_copy(map);
    printf("map2->length = %d\n", Map_length(map));

    wait_return("### Free map");
    Map_free(&map);

    wait_return("### Traverse map2");
    Map_traverse(map2, pr, NULL);

    wait_return("### Free map2");
    Map_free(&map2);

    printf("map = %p, map2 = %p\n", (void *)map, (void *)map2);

    return 0;
}
