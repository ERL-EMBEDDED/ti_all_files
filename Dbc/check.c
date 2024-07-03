#include <stdio.h>
#include <stdint.h>

#define MAX_VALUE 500
#define BITS 16

int main() {
    uint16_t value;

    for (int i = 0; i <= MAX_VALUE; i++) {
        value = (i == MAX_VALUE) ? UINT16_MAX : (uint16_t)((double)i / MAX_VALUE * UINT16_MAX);

        printf("%d in decimal: %d\n", i, value);
    }

    return 0;
}

