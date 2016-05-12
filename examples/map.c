#define _WITH_GETLINE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "mem.h"
#include "atom.h"

#define NCHARS 4
#define NELEMS 1000000

void random_string(char *s, const int len) {
    static const char alphanum[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";
    static const int alphanum_len = sizeof(alphanum) - 1;

    for (int i = 0; i < len - 1; i++) {
	s[i] = alphanum[rand() % alphanum_len];
    }

    s[len - 1] = 0;
}

int cmp(const char *key1, const char *key2, void *cl) {
    // printf("cmp(\"%s\", \"%s\") = %d\n", key1, key2, strcmp(key1, key2));
    return strcmp(key1, key2);
}

char *copy_key(const char *key) {
    char *new_key = CALLOC(NCHARS + 1, sizeof(char));
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
	(Map_copy_fun_T)copy_key, (Map_copy_fun_T)copy_data,
	(Map_free_fun_T)free_key, (Map_free_fun_T)free_data);

    srand(time(NULL));

    wait_return("### Fill map");
    printf("map->length = %zu\n", Map_size(map));
    char *buf;
    int n;

    for (int i = 0; i < NELEMS; i++) {
	n = rand();
	int *np = copy_data(&n);
	buf = CALLOC(NCHARS + 1, sizeof(char));
	random_string(buf, NCHARS+1);
	if (!Map_insert(map, buf, np)) {
	    FREE(buf);
	    FREE(np);
	}
    }
    printf("map->length = %zu\n", Map_size(map));

    wait_return("### Traverse map");
    Map_traverse(map, pr, NULL);

    wait_return("### Copy map -> map2");
    Map_T map2 = Map_copy(map);
    printf("map2->length = %zu\n", Map_size(map));

    wait_return("### Free map");
    Map_free(&map);

    wait_return("### Traverse map2");
    Map_traverse(map2, pr, NULL);

    wait_return("### Free map2");
    Map_free(&map2);

    printf("map = %p, map2 = %p\n", (void *)map, (void *)map2);

    return 0;
}
