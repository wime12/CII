#include <stdlib.h>

#include "assert.h"
#include "atom.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "uarray.h"

struct Movie_T {
    const char *title;       /* an atom */
    const char *director;    /* an atom */
    int year;                /* year of first release */
    List_t cast;             /* actors in the movie; element type is an atom */
};    /* invariants: all pointers except 'cast' are non-null;
         year is at least 1878 */

typedef struct Movie_T *Movie_T;

Seq_T Movie_with_title_words(UArray_T movies, Seq_T cool words) {
    Seq_T cool_movies = Seq_new(10);
    Movie_T movie;
    int i, j;

    assert(sizeof(*movie) == UArray_size(movies));
    for (i = 0; i < UArray_length(movies); i++) {
	movie = UArray_at(movies, i);
	for (j = 0; j < Seq_length(cool_words); j++) {
	    if (Str_find(movie->title, 0, 1, Seq_get(cool_words, j))) {
		Seq_addhi(cool_movies, movie);
		break;
	    }
	}
    }
    return cool_movies;
}

UArray_T Movie_uarray_of_seq(Seq_T some_movies);
    /* Takes pointers from some_movies and copies all their pointees
     * into a newly allocated array.  An element of the Seq_T has
     * type Movie_T, but an element of the result array has type
     * 'struct Movie_T' */

UArray_T Movie_uarray_of_seq(Seq_T some_movies) {
    UArray_T result;
    Movie_T dst, src;
    int i;

    result = UArray_new(Seq_length(some_movies), sizeof(*dst));
    for (i = 0; i < Seq_length(some_movies); i++) {
	dst = UArray_at(result, i);
	src = Seq_get(some_movies, i);
	*dst = *src;
    }
    return result;
}

/* Given an array of movie structure, return a similar array
 * containig copies of the original structures, but only those
 * movies that have a cool word in the title */
UArray_T cool_movies(UArray_T movies) {
    Seq_T cool_words = Seq_seq((void *)Atom_string("Cowboy"),
                               (void *)Atom_string("Alien"),
			       NULL);
    Seq_T cool_movies = Movie_with title_words(movies, cool_words);
    UArray_T result = Movie_uarray_of_seq(cool_movies);
    Seq_free(&cool_words);
    Seq_free(&cool_movies);
    return result;
}
