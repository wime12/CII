#define _WITH_GETLINE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "mem.h"
#include "atom.h"

#define NCHARS 8
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
    FREE(data);
}

int pr(const void *key, const void *n, void *cl) {
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

    printf("map->size = %zu\n", Map_size(map));
    char *buf;
    int n;

    for (int i = 0; i < NELEMS; i++) {
	n = rand();
	int *np = copy_data(&n);
	buf = CALLOC(NCHARS + 1, sizeof(char));
	random_string(buf, NCHARS+1);
	if (!Map_insert(map, buf, np)) {
	    // printf("Collision: \"%s\"\n", buf);
	    FREE(buf);
	    FREE(np);
	}
    }
    printf("map->size = %zu\n", Map_size(map));

    Map_traverse(map, pr, NULL);

    Map_T map2 = Map_copy(map);
    printf("map2->size = %zu\n", Map_size(map));

    Map_free(&map);

    Map_traverse(map2, pr, NULL);

    Map_free(&map2);

    printf("map = %p, map2 = %p\n", (void *)map, (void *)map2);

    return 0;
}
