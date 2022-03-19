#include <stdio.h>
#include "../libpreload.h"


int main() {
    char* i = get_new_map();
    printf("%d", *i);
    free(i);
    return 0;
}
