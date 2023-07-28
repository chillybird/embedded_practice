#include <stdio.h>
#include "embedded_utils.h"

int main() {
    if (isBigEndian()) {
        printf("Is big endian. \n");
    } else {
        printf("Is small endian. \n");
    }
    return 0;
}
