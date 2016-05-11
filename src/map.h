#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#define T Map_T

typedef struct T *T;

extern T Map_new(int (*cmp)(void *key1, void *key2),
          void *(*copy_key)(void *key),
          void *(*copy_data)(void *data),
	  void (*free_key)(void *key),
	  void (*free_data)(void *data)); 

extern T Map_copy(T map);

extern void Map_free(T *mapp);

extern int Map_insert(T map, void *key, void *data);

extern void *Map_remove(T map, void *key);

extern void *Map_get(T map, void *key);

extern void Map_traverse(T map, int (*apply)(void *key, void *data, void *cl),
	void *cl);

#undef T

#endif
