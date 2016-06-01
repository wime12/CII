#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "assert.h"
#include "except.h"

#define T Except_T

Except_Frame *Except_stack = NULL;

void Except_raise(const T *e, const char *file, int line) {
    Except_Frame *p = Except_stack;
    assert(e);
    if (p == NULL) {
	fprintf(STDERR_FILENO, "Uncaught exception");
	if (e->reason)
	    fprintf(STDERR_FILENO, " %s", e->reason);
	else
	    fprintf(STDERR_FILENO, " at 0x%p", (void *)e);
	if (file && line > 0)
	    fprintf(STDERR_FILENO, " raised at %s:%d\n", file, line);
	fprintf(STDERR_FILENO, "aborting...\n");
	fflush(STDERR_FILENO);
	abort();
    }
    p->exception = e;
    p->file = file;
    p->line = line;
    Except_stack = Except_stack->prev;
    longjmp(p->env, Except_raised);
}
