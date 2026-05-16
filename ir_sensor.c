#include "ir.h"

void ir_init(void) {
    DDRC &= ~(1 << PC2);
    DDRC &= ~(1 << PC3);
}

unsigned char ir_left(void) {
    return (PINC & (1 << PC2)) ? 1 : 0;
}

unsigned char ir_right(void) {
    return (PINC & (1 << PC3)) ? 1 : 0;
}
