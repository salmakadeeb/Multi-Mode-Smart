#include "ultrasonic.h"

void ultrasonic_init(void) {
    DDRC |= (1 << PC0);
    DDRC &= ~(1 << PC1);
}

unsigned int get_distance(void) {
    unsigned int count = 0;

    PORTC |= (1 << PC0);
    _delay_us(10);
    PORTC &= ~(1 << PC0);

    unsigned int timeout = 0;
    while (!(PINC & (1 << PC1))) {
        timeout++;
        if (timeout > 30000) return 999;
    }

    while (PINC & (1 << PC1)) {
        count++;
        _delay_us(1);
        if (count > 30000) return 999;
    }

    return count / 58;
}
