#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#define T Map_T

typedef struct T *T;

extern T Map_new(int (*cmp)(const void *key1, const void *key2),
          void *(*copy_key)(const void *key),
	  void *(*copy_data)(const void *data),
	  void (*free_key)(void *key),
	  void (*free_data)(void *data)); 

extern T Map_copy(const T map);

extern void Map_free(T *mapp);

extern int Map_insert(T map, const void *key, const void *data);

extern const void *Map_remove(T map, const void *key);

extern const void *Map_get(const T map, const void *key);

extern void Map_traverse(const T map,
	int (*apply)(const void *key, const void *data, void *cl),
	void *cl);

#undef T

#endif
