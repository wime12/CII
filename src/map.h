#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#define T Map_T

typedef int (*Map_cmp_fun_T)(const void *, const void *);
typedef void *(*Map_copy_fun_T)(const void *);
typedef void (*Map_free_fun_T)(void *);
typedef int (*Map_apply_fun_T)(const void *, const void *, void *);

typedef struct T *T;

extern T Map_new(Map_cmp_fun_T cmp, Map_copy_fun_T copy_key,
	Map_copy_fun_T copy_data, Map_free_fun_T free_key,
	Map_free_fun_T free_data);

extern T Map_copy(const T map);

extern void Map_free(T *mapp);

extern int Map_insert(T map, const void *key, const void *data);

extern const void *Map_remove(T map, const void *key);

extern const void *Map_get(const T map, const void *key);

extern void Map_traverse(const T map, Map_apply_fun_T apply, void *cl);

#undef T

#endif
