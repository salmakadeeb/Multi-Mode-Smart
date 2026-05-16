#ifndef UART_H
#define UART_H

#include <avr/io.h>

void uart_init(void);
unsigned char uart_available(void);
unsigned char uart_read(void);

#endif
