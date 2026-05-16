#ifndef IR_H
#define IR_H

#include <avr/io.h>

void ir_init(void);
unsigned char ir_left(void);
unsigned char ir_right(void);

#endif
