#include "ntree.h"
#include <stdlib.h>

int main() {
    NTree_T t = NTree_new(NULL);
    NTree_free(&t, NULL, NULL);
    
    return 0;
}
