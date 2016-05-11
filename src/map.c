#include "map.h"
#include "assert.h"
#include "mem.h"
#include "rbtree.h"

#define T Map_T

struct assoc {
    void *key;
    void *data;
};

struct Map_T {
    RBTree_T tree;
    int (*cmp)(void *key1, void *key2);
    void *(*copy_key)(void *key);
    void *(*copy_data)(void *data);
    struct assoc *(*copy)(struct assoc *a, T map);
    void (*free_key)(void *key);
    void (*free_data)(void *data);
    void (*free)(struct assoc *a, T map);
};

static struct assoc *copy_key_only(struct assoc *a, T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = (map->copy_key)(a->key);
    a_new->data = a->data;
    return a_new;
}

static struct assoc *copy_data_only(struct assoc *a, T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = a->key;
    a_new->data = (map->copy_data)(a->data);
    return a_new;
}

static struct assoc *copy_assoc(struct assoc *a, T map) {
    struct assoc *a_new;
    NEW(a_new);
    a_new->key = (map->copy_key)(a->key);
    a_new->data = (map->copy_data)(a->data);
    return a_new;
}

static struct assoc *identity(struct assoc *a, T map) {
    return a;
}

static void free_assoc(struct assoc *a, T map) {
    map->free_key(a->key);
    map->free_data(a->data);
    FREE(a);
}

static void free_key_only(struct assoc *a, T map) {
    map->free_key(a->key);
    FREE(a);
}

static void free_data_only(struct assoc *a, T map) {
    map->free_data(a->key);
    FREE(a);
}

static void free_assoc_only(struct assoc *a, T map) {
    FREE(a);
}

T Map_new(int (*cmp)(void *key1, void *key2),
          void *(*copy_key)(void *key),
          void *(*copy_data)(void *data),
	  void (*free_key)(void *key),
	  void (*free_data)(void *data)) {
    assert(cmp);
    T map;
    NEW(map);
    *map = (struct Map_T) {
	.tree = NULL,
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
	map->copy = identity;

    if (free_key && free_data)
	map->free = free_assoc;
    else if (free_key)
	map->free = free_key_only;
    else if (free_key)
	map->free = free_data_only;
    else
	map->free = free_assoc_only;

    return map;
}

T Map_copy(T map) {
    T new_map;
    NEW(new_map);
    *new_map = (struct Map_T) {
	.cmp = map->cmp,
	.copy_key = map->copy_key,
	.copy_data = map->copy_data,
	.copy = map->copy,
	.free_key = map->free_key,
	.free_data = map->free_data,
	.free = map->free,
	.tree = RBTree_copy(map->tree, (void *(*)(void *, void *))map->copy,
	    map)
    };
    return new_map;
}

void Map_free(T *mapp) {
    RBTree_free(&(*mapp)->tree, (void (*)(void *, void *))(*mapp)->free, *mapp);
    FREE(*mapp);
}

int Map_insert(T map, void *key, void *data) {
    struct assoc *a;
    int inserted;
    NEW(a);
    *a = (struct assoc) { .key = key, .data = data };
    inserted = RBTree_insert(&map->tree, a,
	(int (*)(void *, void *, void *))map->cmp, map);
    if (!inserted) FREE(a);
    return inserted;
}

void *Map_remove(T map, void *key) {
    return RBTree_remove(&map->tree, key,
	(int (*)(void *, void *, void *))map->cmp, map);
}

void *Map_get(T map, void *key) {
    struct assoc *a;
    a = RBTree_get(map->tree, key, (int (*)(void *, void *, void *))map->cmp,
	map);
    return a ? a->data : NULL;
}

struct apply_cl {
    int (*apply)(void *key, void *data, void *cl);
    void *cl;
};

int apply_assoc(struct assoc *a, struct apply_cl *apply_cl) {
    return (apply_cl->apply)(a->key, a->data, apply_cl->cl);
}

void Map_traverse(T map, int(*apply)(void *key, void *data, void *cl),
	void *cl) {
    struct apply_cl acl = (struct apply_cl){ .apply = apply, .cl = cl };
    RBTree_traverse(map->tree, (int (*)(void *, void *))apply_assoc, &acl);
}

#undef T
