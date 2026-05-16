#include "uart.h"

void uart_init(void) {
    UCSRA = 0;
    UBRRL = 103;
    UBRRH = 0;
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
    UCSRB = (1 << RXEN);
}

unsigned char uart_available(void) {
    return (UCSRA & (1 << RXC));
}

unsigned char uart_read(void) {
    return UDR;
}
