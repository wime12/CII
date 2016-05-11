#ifndef UARRAYREP_INCLUDED
#define UARRAYREP_INCLUDED

#define T UArray_T

struct T {
    int length;		/* number of elements in 'elems', at least 0 */
    int size;		/* number of bytes in one element */
    char *elems;	/* iff length > 0, pointer to (length * size) bytes */
};

extern void UArrayRep_init(T uarray, int length, int size, void *elems);

#undef T

#endif
