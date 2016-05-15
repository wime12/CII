#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "uarray.h"
#include "uarrayrep.h"
#include "mem.h"

#define T UArray_T

T UArray_new(int length, int size) {
    T array;
    NEW(array);
    if (length > 0)
	UArrayRep_init(array, length, size, CALLOC(length, size));
    else
	UArrayRep_init(array, length, size, NULL);
    return array;
}

void UArrayRep_init(T uarray, int length, int size, void *elems) {
    assert(uarray);
    assert((elems && length > 0) || (length == 0 && elems == NULL));
    assert(size > 0);
    uarray->length = length;
    uarray->size = size;
    if (length > 0)
	uarray->elems = elems;
    else
	uarray->elems = NULL;
}

void UArray_free(T *uarray) {
    assert(uarray && *uarray);
    FREE((*uarray)->elems);
    FREE(*uarray);
}

void *UArray_at(T uarray, int i) {
    assert(uarray);
    assert(i >= 0 && i < uarray->length);
    return uarray->elems + i * uarray->size;
}

int UArray_length(T uarray) {
    assert(uarray);
    return uarray->length;
}

int UArray_size(T uarray) {
    assert(uarray);
    return uarray->size;
}

void UArray_resize(T uarray, int length) {
    assert(uarray);
    assert(length >= 0);
    if (length == 0)
	FREE(uarray->elems);
    else if (uarray->length == 0)
	uarray->elems = ALLOC(length * uarray->size);
    else
	RESIZE(uarray->elems, length * uarray->size);
    uarray->length = length;
}

T UArray_copy(T uarray, int length) {
    T copy;
    assert(uarray);
    assert(length >= 0);
    copy = UArray_new(length, uarray->size);
    if (copy->length >= uarray->length && uarray->length > 0)
	memcpy(copy->elems, uarray->elems, uarray->length * uarray->size);
    else if (uarray->length > copy->length && copy->length > 0)
	memcpy(copy->elems, uarray->elems, copy->length * uarray->size);
    return copy;
}
