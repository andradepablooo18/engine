#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Engine* e = NULL;

    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }

    Engine_run(e);

    Engine_destroy(&e);

    return EXIT_SUCCESS;
}
