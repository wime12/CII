#include "map.h"
#include "assert.h"
#include "mem.h"
#include "rbtree.h"
#include <stdlib.h>

#define T Map_T

struct assoc {
    const void *key;
    const void *data;
};

struct Map_T {
    RBTree_T tree;
    size_t len;
    Map_cmp_fun_T cmp;
    Map_copy_fun_T copy_key;
    Map_copy_fun_T copy_data;
    struct assoc *(*copy)(const struct assoc *a, const T map);
    Map_free_fun_T free_key;
    Map_free_fun_T free_data;
    void (*free)(struct assoc *a, const T map);
};

static struct assoc *copy_key_only(const struct assoc *a, const T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = (map->copy_key)(a->key);
    a_new->data = a->data;
    return a_new;
}

static struct assoc *copy_data_only(const struct assoc *a, const T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = a->key;
    a_new->data = (map->copy_data)(a->data);
    return a_new;
}

static struct assoc *copy_assoc(const struct assoc *a, const T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = (map->copy_key)(a->key);
    a_new->data = (map->copy_data)(a->data);
    return a_new;
}

static struct assoc *identity(struct assoc *a, const T map) {
    return (struct assoc *)a;
}

static void free_assoc(struct assoc *a, T map) {
    map->free_key((void *)a->key);
    map->free_data((void *)a->data);
    FREE(a);
}

static void free_key_only(struct assoc *a, T map) {
    map->free_key((void *)a->key);
    FREE(a);
}

static void free_data_only(struct assoc *a, T map) {
    map->free_data((void *)a->data);
    FREE(a);
}

static void free_assoc_only(struct assoc *a, T map) {
    FREE(a);
}

T Map_new(Map_cmp_fun_T cmp,
          Map_copy_fun_T copy_key, Map_copy_fun_T copy_data,
	  Map_free_fun_T free_key, Map_free_fun_T free_data) {
    assert(cmp);
    T map;
    NEW(map);
    *map = (struct Map_T) {
	.tree = NULL,
	.len = 0,
	.cmp = cmp,
	.copy_key = copy_key,
	.copy_data = copy_data,
	.free_key = free_key,
	.free_data = free_data,
    };

    if (copy_key && copy_data)
	map->copy = copy_assoc;
    else if (copy_data)
	map->copy = copy_data_only;
    else if (copy_key)
	map->copy = copy_key_only;
    else
	map->copy = (struct assoc *(*)(const struct assoc *, const T))identity;

    if (free_key && free_data)
	map->free = free_assoc;
    else if (free_key)
	map->free = free_key_only;
    else if (free_data)
	map->free = free_data_only;
    else
	map->free = free_assoc_only;

    return map;
}

T Map_copy(T map) {
    T new_map;
    NEW(new_map);
    *new_map = (struct Map_T) {
	.len = map->len,
	.cmp = map->cmp,
	.copy_key = map->copy_key,
	.copy_data = map->copy_data,
	.copy = map->copy,
	.free_key = map->free_key,
	.free_data = map->free_data,
	.free = map->free,
	.tree = RBTree_copy(map->tree,
		(void *(*)(const void *, void *))map->copy,
	    map)
    };
    return new_map;
}

void Map_free(T *mapp) {
    assert(mapp && *mapp);
    RBTree_free(&(*mapp)->tree, (void (*)(void *, void *))(*mapp)->free, *mapp);
    FREE(*mapp);
}

size_t Map_size(const T map) {
    assert(map);
    return map->len;
}

static int map_cmp(const struct assoc *a1, const struct assoc *a2, T map) {
    return map->cmp(a1->key, a2->key);
}

int Map_insert(T map, const void *key, const void *data) {
    struct assoc *a;
    int inserted;
    NEW(a);
    *a = (struct assoc) { .key = key, .data = data };
    inserted = RBTree_insert(&map->tree, a,
	(int (*)(const void *, const void *, void *))map_cmp, map);
    if (inserted)
	map->len++;
    else
	FREE(a);
    return inserted;
}

const void *Map_remove(T map, const void *key) {
    const struct assoc *a = RBTree_remove(&map->tree, key,
	(int (*)(const void *, const void *, void *))map_cmp, map);
    if (a) {
	map->len--;
	return a->data;
    }
    else
	return NULL;
}

const void *Map_get(const T map, const void *key) {
    const struct assoc *a = RBTree_get(map->tree, key,
	    (int (*)(const void *, const void *, void *))map->cmp,
	    map);
    return a ? a->data : NULL;
}

struct apply_cl {
    int (*apply)(const void *key, const void *data, void *cl);
    void *cl;
};

int apply_assoc(struct assoc *a, struct apply_cl *apply_cl) {
    return (apply_cl->apply)(a->key, a->data, apply_cl->cl);
}

void Map_traverse(const T map, Map_apply_fun_T apply, void *cl) {
    struct apply_cl acl = (struct apply_cl){ .apply = apply, .cl = cl };
    RBTree_traverse(map->tree,
	(int (*)(const void *, void *))apply_assoc, &acl);
}

#undef T
